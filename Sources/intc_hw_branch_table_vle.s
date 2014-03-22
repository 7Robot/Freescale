# intc_hw_branch_table_vle.s - INTC hardware vector mode branch table example
# Description: INTC vector branch table when using INTC in HW vector mode
#            **** NOTE **** ONLY 100 EXAMPLE VECTORS ARE IMPLEMENTED HERE
# Rev 1.0 Jul  2, 2007 S Mihalik 
# Rev 1.1 Aug 30 1007 SM - Made SwIrq4Handler, emiosCh0Handler .extern 
# Rev 2.0 Jan 22 2009 SM - Modified for VLE and MPC56xxB/P/S INTC vector numbers
# Copyright Freescale Semiconductor, Inc. 2007. All rights reserved

  .section .intc_hw_branch_table,text_vle
  .extern SwIrq4Handler 
  .extern Pit1Handler
  #.extern Pit2Handler
  .extern UART_TXI_Handler
  .extern UART_RXI_Handler
  .extern Capteur1_Roue_Handler
  .extern Capteur2_Roue_Handler
  
.equ ALIGN_OFFSET, 2  # MPC551x,MPC56xxB/P/S: 4 byte branch alignments (Diab/GHS use 2; CW 4)
#.equ ALIGN_OFFSET, 4 # MPC555x: 16 byte branch alignments (Diab/GHS use 4; CW 16)
 
IntcHandlerBranchTable: # Only 100 example vectors are implemented here
                        # MPC555x: This table must have 64 KB alignment
                        # MPC551x, MPC56xxB/P/S: Requires 2 KB alignment after 4KB boundary

 .align ALIGN_OFFSET
e_b hw_vect0        #INTC HW vector 0
.align ALIGN_OFFSET
e_b hw_vect1        #INTC HW vector 1
.align ALIGN_OFFSET
e_b hw_vect2        #INTC HW vector 2
.align ALIGN_OFFSET
e_b hw_vect3        #INTC HW vector 3
.align ALIGN_OFFSET
e_b SwIrq4Handler        #INTC HW vector 4 modifiee
.align ALIGN_OFFSET
e_b hw_vect5        #INTC HW vector 5
.align ALIGN_OFFSET
e_b hw_vect6        #INTC HW vector 6
.align ALIGN_OFFSET
e_b hw_vect7        #INTC HW vector 7
.align ALIGN_OFFSET
e_b hw_vect8        #INTC HW vector 8
.align ALIGN_OFFSET
e_b hw_vect9        #INTC HW vector 9
.align ALIGN_OFFSET
e_b hw_vect10        #INTC HW vector 10
.align ALIGN_OFFSET
e_b hw_vect11        #INTC HW vector 11
.align ALIGN_OFFSET
e_b hw_vect12        #INTC HW vector 12
.align ALIGN_OFFSET
e_b hw_vect13        #INTC HW vector 13
.align ALIGN_OFFSET
e_b hw_vect14        #INTC HW vector 14
.align ALIGN_OFFSET
e_b hw_vect15        #INTC HW vector 15
.align ALIGN_OFFSET
e_b hw_vect16        #INTC HW vector 16
.align ALIGN_OFFSET
e_b hw_vect17        #INTC HW vector 17
.align ALIGN_OFFSET
e_b hw_vect18        #INTC HW vector 18
.align ALIGN_OFFSET
e_b hw_vect19        #INTC HW vector 19
.align ALIGN_OFFSET
e_b hw_vect20        #INTC HW vector 20
.align ALIGN_OFFSET
e_b hw_vect21        #INTC HW vector 21
.align ALIGN_OFFSET
e_b hw_vect22        #INTC HW vector 22
.align ALIGN_OFFSET
e_b hw_vect23        #INTC HW vector 23
.align ALIGN_OFFSET
e_b hw_vect24        #INTC HW vector 24
.align ALIGN_OFFSET
e_b hw_vect25        #INTC HW vector 25
.align ALIGN_OFFSET
e_b hw_vect26        #INTC HW vector 26
.align ALIGN_OFFSET
e_b hw_vect27        #INTC HW vector 27
.align ALIGN_OFFSET
e_b hw_vect28        #INTC HW vector 28
.align ALIGN_OFFSET
e_b hw_vect29        #INTC HW vector 29
.align ALIGN_OFFSET
e_b hw_vect30        #INTC HW vector 30
.align ALIGN_OFFSET
e_b hw_vect31        #INTC HW vector 31
.align ALIGN_OFFSET
e_b hw_vect32        #INTC HW vector 32
.align ALIGN_OFFSET
e_b hw_vect33        #INTC HW vector 33
.align ALIGN_OFFSET
e_b hw_vect34        #INTC HW vector 34
.align ALIGN_OFFSET
e_b hw_vect35        #INTC HW vector 35
.align ALIGN_OFFSET
e_b hw_vect36        #INTC HW vector 36
.align ALIGN_OFFSET
e_b hw_vect37        #INTC HW vector 37
.align ALIGN_OFFSET
e_b hw_vect38        #INTC HW vector 38
.align ALIGN_OFFSET
e_b hw_vect39        #INTC HW vector 39
.align ALIGN_OFFSET
e_b hw_vect40        #INTC HW vector 40
.align ALIGN_OFFSET
e_b hw_vect41        #INTC HW vector 41
.align ALIGN_OFFSET
e_b hw_vect42        #INTC HW vector 42
.align ALIGN_OFFSET
e_b hw_vect43        #INTC HW vector 43
.align ALIGN_OFFSET
e_b hw_vect44        #INTC HW vector 44
.align ALIGN_OFFSET
e_b hw_vect45        #INTC HW vector 45
.align ALIGN_OFFSET
e_b hw_vect46        #INTC HW vector 46
.align ALIGN_OFFSET
e_b hw_vect47        #INTC HW vector 47
.align ALIGN_OFFSET
e_b hw_vect48        #INTC HW vector 48
.align ALIGN_OFFSET
e_b hw_vect49        #INTC HW vector 49
.align ALIGN_OFFSET
e_b hw_vect50        #INTC HW vector 50
.align ALIGN_OFFSET
e_b hw_vect51        #INTC HW vector 51
.align ALIGN_OFFSET
e_b hw_vect52        #INTC HW vector 52
.align ALIGN_OFFSET
e_b hw_vect53        #INTC HW vector 53
.align ALIGN_OFFSET
e_b hw_vect54        #INTC HW vector 54
.align ALIGN_OFFSET
e_b hw_vect55        #INTC HW vector 55
.align ALIGN_OFFSET
e_b hw_vect56        #INTC HW vector 56
.align ALIGN_OFFSET
e_b hw_vect57        #INTC HW vector 57
.align ALIGN_OFFSET
e_b hw_vect58        #INTC HW vector 58
.align ALIGN_OFFSET
e_b hw_vect59        #INTC HW vector 59
.align ALIGN_OFFSET
e_b Pit1Handler        #INTC HW vector 60  modifiee
.align ALIGN_OFFSET
e_b hw_vect61        #INTC HW vector 61
.align ALIGN_OFFSET
e_b hw_vect62        #INTC HW vector 62
.align ALIGN_OFFSET
e_b hw_vect63        #INTC HW vector 63
.align ALIGN_OFFSET
e_b hw_vect64        #INTC HW vector 64
.align ALIGN_OFFSET
e_b hw_vect65        #INTC HW vector 65
.align ALIGN_OFFSET
e_b hw_vect66        #INTC HW vector 66
.align ALIGN_OFFSET
e_b hw_vect67        #INTC HW vector 67
.align ALIGN_OFFSET
e_b hw_vect68        #INTC HW vector 68
.align ALIGN_OFFSET
e_b hw_vect69        #INTC HW vector 69
.align ALIGN_OFFSET
e_b hw_vect70        #INTC HW vector 70
.align ALIGN_OFFSET
e_b hw_vect71        #INTC HW vector 71
.align ALIGN_OFFSET
e_b hw_vect72        #INTC HW vector 72
.align ALIGN_OFFSET
e_b hw_vect73        #INTC HW vector 73
.align ALIGN_OFFSET
e_b hw_vect74        #INTC HW vector 74
.align ALIGN_OFFSET
e_b hw_vect75        #INTC HW vector 75
.align ALIGN_OFFSET
e_b hw_vect76        #INTC HW vector 76
.align ALIGN_OFFSET
e_b hw_vect77        #INTC HW vector 77
.align ALIGN_OFFSET
e_b hw_vect78        #INTC HW vector 78
.align ALIGN_OFFSET
e_b UART_RXI_Handler		 	#INTC HW vector 79 modifiée
.align ALIGN_OFFSET
e_b UART_TXI_Handler        	#INTC HW vector 80 modifiée
.align ALIGN_OFFSET
e_b hw_vect81        #INTC HW vector 81
.align ALIGN_OFFSET
e_b hw_vect82        #INTC HW vector 82
.align ALIGN_OFFSET
e_b hw_vect83        #INTC HW vector 83
.align ALIGN_OFFSET
e_b hw_vect84        #INTC HW vector 84
.align ALIGN_OFFSET
e_b hw_vect85        #INTC HW vector 85
.align ALIGN_OFFSET
e_b hw_vect86        #INTC HW vector 86
.align ALIGN_OFFSET
e_b hw_vect87        #INTC HW vector 87
.align ALIGN_OFFSET
e_b hw_vect88        #INTC HW vector 88
.align ALIGN_OFFSET
e_b hw_vect89        #INTC HW vector 89
.align ALIGN_OFFSET
e_b hw_vect90        #INTC HW vector 90
.align ALIGN_OFFSET
e_b hw_vect91        #INTC HW vector 91
.align ALIGN_OFFSET
e_b hw_vect92        #INTC HW vector 92
.align ALIGN_OFFSET
e_b hw_vect93        #INTC HW vector 93
.align ALIGN_OFFSET
e_b hw_vect94        #INTC HW vector 94
.align ALIGN_OFFSET
e_b hw_vect95        #INTC HW vector 95
.align ALIGN_OFFSET
e_b hw_vect96        #INTC HW vector 96
.align ALIGN_OFFSET
e_b hw_vect97        #INTC HW vector 97
.align ALIGN_OFFSET
e_b hw_vect98        #INTC HW vector 98
.align ALIGN_OFFSET
e_b hw_vect99        #INTC HW vector 99
.align ALIGN_OFFSET
e_b hw_vect100        #INTC HW vector 100
.align ALIGN_OFFSET
e_b hw_vect101        #INTC HW vector 101
.align ALIGN_OFFSET
e_b hw_vect102        #INTC HW vector 102
.align ALIGN_OFFSET
e_b hw_vect103        #INTC HW vector 103
.align ALIGN_OFFSET
e_b hw_vect104        #INTC HW vector 104
.align ALIGN_OFFSET
e_b hw_vect105        #INTC HW vector 105
.align ALIGN_OFFSET
e_b hw_vect106        #INTC HW vector 106
.align ALIGN_OFFSET
e_b hw_vect107        #INTC HW vector 107
.align ALIGN_OFFSET
e_b hw_vect108        #INTC HW vector 108
.align ALIGN_OFFSET
e_b hw_vect109        #INTC HW vector 109
.align ALIGN_OFFSET
e_b hw_vect110        #INTC HW vector 110
.align ALIGN_OFFSET
e_b hw_vect111        #INTC HW vector 111
.align ALIGN_OFFSET
e_b hw_vect112        #INTC HW vector 112
.align ALIGN_OFFSET
e_b hw_vect113        #INTC HW vector 113
.align ALIGN_OFFSET
e_b hw_vect114        #INTC HW vector 114
.align ALIGN_OFFSET
e_b hw_vect115        #INTC HW vector 115
.align ALIGN_OFFSET
e_b hw_vect116        #INTC HW vector 116
.align ALIGN_OFFSET
e_b hw_vect117        #INTC HW vector 117
.align ALIGN_OFFSET
e_b hw_vect118        #INTC HW vector 118
.align ALIGN_OFFSET
e_b hw_vect119        #INTC HW vector 119
.align ALIGN_OFFSET
e_b hw_vect120        #INTC HW vector 120
.align ALIGN_OFFSET
e_b hw_vect121        #INTC HW vector 121
.align ALIGN_OFFSET
e_b UART_RXI_Handler        	#INTC HW vector 122 modifiée
.align ALIGN_OFFSET
e_b hw_vect123        #INTC HW vector 123
.align ALIGN_OFFSET
e_b hw_vect124        #INTC HW vector 124
.align ALIGN_OFFSET
e_b hw_vect125        #INTC HW vector 125
.align ALIGN_OFFSET
e_b hw_vect126        #INTC HW vector 126
.align ALIGN_OFFSET
e_b hw_vect127        #INTC HW vector 127
.align ALIGN_OFFSET
e_b hw_vect128        #INTC HW vector 128
.align ALIGN_OFFSET
e_b hw_vect129        #INTC HW vector 129
.align ALIGN_OFFSET
e_b hw_vect130        #INTC HW vector 130
.align ALIGN_OFFSET
e_b hw_vect131        #INTC HW vector 131
.align ALIGN_OFFSET
e_b hw_vect132        #INTC HW vector 132
.align ALIGN_OFFSET
e_b hw_vect133        #INTC HW vector 133
.align ALIGN_OFFSET
e_b hw_vect134        #INTC HW vector 134
.align ALIGN_OFFSET
e_b hw_vect135        #INTC HW vector 135
.align ALIGN_OFFSET
e_b hw_vect136        #INTC HW vector 136
.align ALIGN_OFFSET
e_b hw_vect137        #INTC HW vector 137
.align ALIGN_OFFSET
e_b hw_vect138        #INTC HW vector 138
.align ALIGN_OFFSET
e_b hw_vect139        #INTC HW vector 139
.align ALIGN_OFFSET
e_b hw_vect140        #INTC HW vector 140
.align ALIGN_OFFSET
e_b hw_vect141        #INTC HW vector 141
.align ALIGN_OFFSET
e_b hw_vect142        #INTC HW vector 142
.align ALIGN_OFFSET
e_b hw_vect143        #INTC HW vector 143
.align ALIGN_OFFSET
e_b hw_vect144        #INTC HW vector 144
.align ALIGN_OFFSET
e_b hw_vect145        #INTC HW vector 145
.align ALIGN_OFFSET
e_b hw_vect146        #INTC HW vector 146
.align ALIGN_OFFSET
e_b hw_vect147        #INTC HW vector 147
.align ALIGN_OFFSET
e_b hw_vect148        #INTC HW vector 148
.align ALIGN_OFFSET
e_b hw_vect149        #INTC HW vector 149
.align ALIGN_OFFSET
e_b hw_vect150        #INTC HW vector 150
.align ALIGN_OFFSET
e_b hw_vect151        #INTC HW vector 151
.align ALIGN_OFFSET
e_b hw_vect152        #INTC HW vector 152
.align ALIGN_OFFSET
e_b hw_vect153        #INTC HW vector 153
.align ALIGN_OFFSET
e_b hw_vect154        #INTC HW vector 154
.align ALIGN_OFFSET
e_b hw_vect155        #INTC HW vector 155
.align ALIGN_OFFSET
e_b hw_vect156        #INTC HW vector 156
.align ALIGN_OFFSET
e_b hw_vect157        #INTC HW vector 157
.align ALIGN_OFFSET
e_b hw_vect158        #INTC HW vector 158
.align ALIGN_OFFSET
e_b hw_vect159        #INTC HW vector 159
.align ALIGN_OFFSET
e_b hw_vect160        #INTC HW vector 160
.align ALIGN_OFFSET
e_b hw_vect161        #INTC HW vector 161
.align ALIGN_OFFSET
e_b hw_vect162        #INTC HW vector 162
.align ALIGN_OFFSET
e_b Capteur1_Roue_Handler        #INTC HW vector 163 modifié
.align ALIGN_OFFSET
e_b Capteur2_Roue_Handler        #INTC HW vector 164 modifié
.align ALIGN_OFFSET
e_b hw_vect165        #INTC HW vector 165
.align ALIGN_OFFSET
e_b hw_vect166        #INTC HW vector 166
.align ALIGN_OFFSET
e_b hw_vect167        #INTC HW vector 167
.align ALIGN_OFFSET
e_b hw_vect168        #INTC HW vector 168
.align ALIGN_OFFSET
e_b hw_vect169        #INTC HW vector 169
.align ALIGN_OFFSET
e_b hw_vect170        #INTC HW vector 170
.align ALIGN_OFFSET
e_b hw_vect171        #INTC HW vector 171
.align ALIGN_OFFSET
e_b hw_vect172        #INTC HW vector 172
.align ALIGN_OFFSET
e_b hw_vect173        #INTC HW vector 173
.align ALIGN_OFFSET
e_b hw_vect174        #INTC HW vector 174
.align ALIGN_OFFSET
e_b hw_vect175        #INTC HW vector 175
.align ALIGN_OFFSET
e_b hw_vect176        #INTC HW vector 176
.align ALIGN_OFFSET
e_b hw_vect177        #INTC HW vector 177
.align ALIGN_OFFSET
e_b hw_vect178        #INTC HW vector 178
.align ALIGN_OFFSET
e_b hw_vect179        #INTC HW vector 179
.align ALIGN_OFFSET
e_b hw_vect180        #INTC HW vector 180
.align ALIGN_OFFSET
e_b hw_vect181        #INTC HW vector 181
.align ALIGN_OFFSET
e_b hw_vect182        #INTC HW vector 182
.align ALIGN_OFFSET
e_b hw_vect183        #INTC HW vector 183
.align ALIGN_OFFSET
e_b hw_vect184        #INTC HW vector 184
.align ALIGN_OFFSET
e_b hw_vect185        #INTC HW vector 185
.align ALIGN_OFFSET
e_b hw_vect186        #INTC HW vector 186
.align ALIGN_OFFSET
e_b hw_vect187        #INTC HW vector 187
.align ALIGN_OFFSET
e_b hw_vect188        #INTC HW vector 188
.align ALIGN_OFFSET
e_b hw_vect189        #INTC HW vector 189
.align ALIGN_OFFSET
e_b hw_vect190        #INTC HW vector 190
.align ALIGN_OFFSET
e_b hw_vect191        #INTC HW vector 191
.align ALIGN_OFFSET
e_b hw_vect192        #INTC HW vector 192
.align ALIGN_OFFSET
e_b hw_vect193        #INTC HW vector 193
.align ALIGN_OFFSET
e_b hw_vect194        #INTC HW vector 194
.align ALIGN_OFFSET
e_b hw_vect195        #INTC HW vector 195
.align ALIGN_OFFSET
e_b hw_vect196        #INTC HW vector 196
.align ALIGN_OFFSET
e_b hw_vect197        #INTC HW vector 197
.align ALIGN_OFFSET
e_b hw_vect198        #INTC HW vector 198
.align ALIGN_OFFSET
e_b hw_vect199        #INTC HW vector 199
.align ALIGN_OFFSET
e_b hw_vect200        #INTC HW vector 200
.align ALIGN_OFFSET
e_b hw_vect201        #INTC HW vector 201
.align ALIGN_OFFSET
e_b hw_vect202        #INTC HW vector 202
.align ALIGN_OFFSET
e_b hw_vect203        #INTC HW vector 203
.align ALIGN_OFFSET
e_b hw_vect204        #INTC HW vector 204
.align ALIGN_OFFSET
e_b hw_vect205        #INTC HW vector 205
.align ALIGN_OFFSET
e_b hw_vect206        #INTC HW vector 206
.align ALIGN_OFFSET
e_b hw_vect207        #INTC HW vector 207
.align ALIGN_OFFSET
e_b hw_vect208        #INTC HW vector 208
.align ALIGN_OFFSET
e_b hw_vect209        #INTC HW vector 209
.align ALIGN_OFFSET
e_b hw_vect210        #INTC HW vector 210
.align ALIGN_OFFSET
e_b hw_vect211        #INTC HW vector 211
.align ALIGN_OFFSET
e_b hw_vect212        #INTC HW vector 212
.align ALIGN_OFFSET
e_b hw_vect213        #INTC HW vector 213
.align ALIGN_OFFSET
e_b hw_vect214        #INTC HW vector 214
.align ALIGN_OFFSET
e_b hw_vect215        #INTC HW vector 215
.align ALIGN_OFFSET
e_b hw_vect216        #INTC HW vector 216



hw_vect0:
hw_vect1:
hw_vect2:
hw_vect3:
hw_vect4:
hw_vect5:
hw_vect6:
hw_vect7:
hw_vect8:
hw_vect9:
hw_vect10:
hw_vect11:
hw_vect12:
hw_vect13:
hw_vect14:
hw_vect15:
hw_vect16:
hw_vect17:
hw_vect18:
hw_vect19:
hw_vect20:
hw_vect21:
hw_vect22:
hw_vect23:
hw_vect24:
hw_vect25:
hw_vect26:
hw_vect27:
hw_vect28:
hw_vect29:
hw_vect30:
hw_vect31:
hw_vect32:
hw_vect33:
hw_vect34:
hw_vect35:
hw_vect36:
hw_vect37:
hw_vect38:
hw_vect39:
hw_vect40:
hw_vect41:
hw_vect42:
hw_vect43:
hw_vect44:
hw_vect45:
hw_vect46:
hw_vect47:
hw_vect48:
hw_vect49:
hw_vect50:
hw_vect51:
hw_vect52:
hw_vect53:
hw_vect54:
hw_vect55:
hw_vect56:
hw_vect57:
hw_vect58:
hw_vect59:
hw_vect60:
hw_vect61:
hw_vect62:
hw_vect63:
hw_vect64:
hw_vect65:
hw_vect66:
hw_vect67:
hw_vect68:
hw_vect69:
hw_vect70:
hw_vect71:
hw_vect72:
hw_vect73:
hw_vect74:
hw_vect75:
hw_vect76:
hw_vect77:
hw_vect78:
hw_vect79:
hw_vect80:
hw_vect81:
hw_vect82:
hw_vect83:
hw_vect84:
hw_vect85:
hw_vect86:
hw_vect87:
hw_vect88:
hw_vect89:
hw_vect90:
hw_vect91:
hw_vect92:
hw_vect93:
hw_vect94:
hw_vect95:
hw_vect96:
hw_vect97:
hw_vect98:
hw_vect99:
hw_vect100:
hw_vect101:
hw_vect102:
hw_vect103:
hw_vect104:
hw_vect105:
hw_vect106:
hw_vect107:
hw_vect108:
hw_vect109:
hw_vect110:
hw_vect111:
hw_vect112:
hw_vect113:
hw_vect114:
hw_vect115:
hw_vect116:
hw_vect117:
hw_vect118:
hw_vect119:
hw_vect120:
hw_vect121:
hw_vect122:
hw_vect123:
hw_vect124:
hw_vect125:
hw_vect126:
hw_vect127:
hw_vect128:
hw_vect129:
hw_vect130:
hw_vect131:
hw_vect132:
hw_vect133:
hw_vect134:
hw_vect135:
hw_vect136:
hw_vect137:
hw_vect138:
hw_vect139:
hw_vect140:
hw_vect141:
hw_vect142:
hw_vect143:
hw_vect144:
hw_vect145:
hw_vect146:
hw_vect147:
hw_vect148:
hw_vect149:
hw_vect150:
hw_vect151:
hw_vect152:
hw_vect153:
hw_vect154:
hw_vect155:
hw_vect156:
hw_vect157:
hw_vect158:
hw_vect159:
hw_vect160:
hw_vect161:
hw_vect162:
hw_vect163:
hw_vect164:
hw_vect165:
hw_vect166:
hw_vect167:
hw_vect168:
hw_vect169:
hw_vect170:
hw_vect171:
hw_vect172:
hw_vect173:
hw_vect174:
hw_vect175:
hw_vect176:
hw_vect177:
hw_vect178:
hw_vect179:
hw_vect180:
hw_vect181:
hw_vect182:
hw_vect183:
hw_vect184:
hw_vect185:
hw_vect186:
hw_vect187:
hw_vect188:
hw_vect189:
hw_vect190:
hw_vect191:
hw_vect192:
hw_vect193:
hw_vect194:
hw_vect195:
hw_vect196:
hw_vect197:
hw_vect198:
hw_vect199:
hw_vect200:
hw_vect201:
hw_vect202:
hw_vect203:
hw_vect204:
hw_vect205:
hw_vect206:
hw_vect207:
hw_vect208:
hw_vect209:
hw_vect210:
hw_vect211:
hw_vect212:
hw_vect213:
hw_vect214:
hw_vect215:
hw_vect216:
boucle_eternelle:
e_b boucle_eternelle

