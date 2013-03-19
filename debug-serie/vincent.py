    #!/usr/bin/python2
    # -*- coding: utf-8 -*-
     
    import Tkinter, random, time
    import socket, sys
    import random, time, threading
    from struct import unpack
     
     
    #########################    PARAMÈTRES    #####################################
     
    # Dimmensions du graphique en pixel
    canvas_height = 600
    canvas_width = 800
     
    # Valeurs min et max de l'axe des ordonnées
    val_min = -200
    val_max = 200
     
    # Temps de raffraichissement de l'écran en secondes
    refresh = 0.01
     
    # Couleurs des graphes
    colors = ["#00FF00", "#FF0000", "#0000FF", "#FFFF00", "#00FFFF", "#FF00FF"]
     
    # Entrées, valeurs acceptées :
    #  -> stdin  : récupération des valeurs via l'entrée standard
    #       utilisation ./script_qui_affiche_mes_données_ligne_par_ligne | ./real_time_plot.py
    #  -> random : valeurs aléatoires, pour les tests
    #  -> serial : port série
    entree = "random"
     
    if entree == "serial":
        import serial
        # Configuration du port série
        port = '/dev/pts/13' # sous Windows : COM13
        baudrate = 9600
        # Format binaire des données
        # définit par http://docs.python.org/2/library/struct.html#format-characters
        format = "Ihh"
        # taille en octets du format,
        #    exemple, si format = "hh" (2 shorts, 2 octets chacun), alors taille_format = 4
        taille_format = 8
     
     
    ############################    FIN PARAMÈTRES     #############################
     
    def convert(val):
        return canvas_height - canvas_height*float(val-val_min)/(val_max-val_min)
       
    def convert2(val):
        return (canvas_height - val)/512.0*(val_max-val_min) + val_min
       
     
     
    class App:
     
        def white(self):
            self.lines=[]
            self.lastpos=[]
     
            self.c.create_rectangle(0, 0, canvas_width, canvas_height, fill="black")
           
            self.gradations()
           
        def gradations(self):
            # Vertical gradations
            for y in range(0,canvas_height,32):
                    self.c.create_line(0, y, canvas_width, y, fill="#333333",dash=(4, 4))
                    self.c.create_text(5, y-10, fill="#999999", text=str(convert2(y)), anchor="w")
            # Horizontal gradations
            for x in range(100,canvas_width,100):
                    self.c.create_line(x, 0, x, canvas_height, fill="#333333",dash=(4, 4))
                    self.c.create_text(x+3, canvas_height-22, fill="#999999", text=str(x), anchor="w")
     
            self.lineRedraw=self.c.create_line(0, canvas_width, 0, 0, fill="red")
     
            self.lines1text=self.c.create_text(canvas_width-150, 10, fill="#00FF00", text=str("TEST"), anchor="e")
     
        def addPoint(self,vals):
            for i in range(len(vals)):
                val = vals[i]  
                if len(self.data) <= i:
                    self.data.append([0]*canvas_width)
                    self.lastpos.append(0)
                    self.lines.append([])
                    for x in range(canvas_width):
                        self.lines[i].append(self.c.create_line(x, 0, x, 0, fill=colors[i%len(colors)]))
                   
                self.data[i][self.xpos] = val
                # Display the average value
                #~ self.line1avg+=val
                #~ if self.xpos%10==0:
                        #~ self.c.itemconfig(self.lines1text,text="Average: "+str(self.line1avg/10.0))
                        #~ self.line1avg=0
                if self.xpos>0:
                    self.c.coords(self.lines[i][self.xpos],(self.xpos-1,self.lastpos[i],self.xpos,convert(val)))
                if self.xpos<canvas_width:
                    self.c.coords(self.lineRedraw,(self.xpos+1,0,self.xpos+1,canvas_width))
                self.lastpos[i]=convert(val)
            self.xpos+=1
            if self.xpos==canvas_width:
                    self.xpos=0
                    self.totalPoints+=canvas_width
                    print "FPS:",self.totalPoints/(time.time()-self.timeStart)
            t.update()
     
        def __init__(self, t):
            self.xpos=0
            self.line1avg=0
            self.data = []
            self.c = Tkinter.Canvas(t, width=canvas_width, height=canvas_height)
            self.c.pack()
            self.totalPoints=0
            self.white()
            self.timeStart=time.time()
                   
     
     
     
    class ValuesReaderSerial:
        def __init__(self):
            self.ser = serial.Serial(
                port=port,
                baudrate=baudrate,
                parity=serial.PARITY_ODD,
                stopbits=serial.STOPBITS_TWO,
                bytesize=serial.SEVENBITS
            )
            if not self.ser.isOpen():
                self.ser.open()
            print "Serial port open"
               
        def get(self):
            if self.ser.inWaiting() > 0:
                buff = self.ser.read(taille_format);
                values = unpack(format, buff)
                print(values)
                return values
            else:
                 return None
     
     
    class ValuesReaderRandom:
        def __init__(self):
            self.val1 = 20
            self.val2 = 30
            self.val3 = 40
        def get(self):
            self.val1 += random.randint(-1,1)
            self.val2 += random.randint(-3,3)
            self.val3 += random.randint(-2,2)
            return (self.val1, self.val2, self.val3)
       
       
    class ValuesReaderStdIn:
        def get(self):
            line = sys.stdin.readline()
            vals = map(float, line.split())
            return vals
     
     
     
     
     
    if entree == "random":
        values = ValuesReaderRandom()
    elif entree == "stdin":
        values = ValuesReaderStdIn()
    elif entree == "serial":
        values = ValuesReaderSerial()
    else:
        RuntimeError("entree invalide")
     
     
    t = Tkinter.Tk()
    a = App(t)
     
    # Boucle principale
    while True:
        points = values.get()
        if points != None:
            print points
            a.addPoint(points)
        else:
            t.update()
        time.sleep(refresh)
