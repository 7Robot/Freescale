# -*- coding: utf-8 -*-
import serial
import struct
from collections import OrderedDict
import output_modules

port = '/dev/ttyUSB0'
baudrate = 115200

fields = OrderedDict()

fields.update(
    ('graph%s' % i, 'b') for i in range(125)
)

fields['pos_min'] = 'B'
fields['pos_max'] = 'B'
fields['incertitude'] = 'B'
fields['milieu_ligne'] = 'B'

output = (
    ('Graph', set('graph%s' % i for i in range(125))),
    #('MovingGraph', set(['incertitude', 'milieu_ligne'])),
    ('Stdout', ['incertitude', 'milieu_ligne', 'pos_min', 'pos_max'])
    )

s = serial.Serial(
    port=port,
    baudrate=baudrate,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS)

if not s.isOpen():
    s.open()
 
input_format = struct.Struct(''.join(fields.values()))

output_objects = [getattr(output_modules, i)() for i, _ in output]

while struct.unpack('B', s.read(1))[0] != 0x42:
    pass

while True:
    unpacked_fields = input_format.unpack(s.read(input_format.size))

    assert struct.unpack('B', s.read(1))[0] == 0x42, 'Il y a l\'octet magique dans les données envoyées, ou elles ne correspondent pas à celles qu\'on veut reçevoir !'

    named_fields = OrderedDict((name, field) for name, field in zip(fields.keys(), unpacked_fields))

    for out, (_, f) in zip(output_objects, output):
        out.update(OrderedDict((name, value) for name, value in named_fields.items() if name in f))

