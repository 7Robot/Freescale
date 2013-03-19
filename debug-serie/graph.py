# -*- coding: utf-8 -*-
import serial
import struct
import random
from collections import OrderedDict
import output_modules

port = '/dev/ttyUSB0'
baudrate = 115200

fields = OrderedDict(
    ('graph%s' % i, 'b') for i in range(125)
)
output = (
    ('Stdout', {'graph1', 'graph2'}),
    ('Graph', set('graph%s' % i for i in range(125)))
    )

s = serial.Serial(
    port=port,
    baudrate=baudrate,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.SEVENBITS)

if not s.isOpen():
    s.open()

input_format = struct.Struct(''.join(fields.values()))

output_objects = [getattr(output_modules, i)() for i, _ in output]

while struct.unpack('b', s.read(1))[0] != 0:
    pass

while True:
    unpacked_fields = input_format.unpack(s.read(input_format.size))

    assert struct.unpack('b', s.read(1))[0] == 0, 'Il y a des caractères nuls dans les données envoyées, ou elles ne correspondent pas à celles qu\'on veut reçevoir !'

    named_fields = OrderedDict((name, field) for name, field in zip(fields.keys(), unpacked_fields))

    for out, (_, f) in zip(output_objects, output):
        out.update(OrderedDict((name, value) for name, value in named_fields.items() if name in f))

