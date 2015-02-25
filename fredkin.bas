10 DEF SEG = &HB800
15 DEFINT A-Y
20 SCREEN 0,0,0
30 CLS
40 DIM A(25,80),B(25,80)
50 REM 219 IS ASCII FOR WHITE PIXEL
60 REM 255 IS ASCII FOR BLACK PIXEL
70 FOR I%=0 TO 24
80 FOR J%=0 TO 79
100 IF I%=12 AND J%=40 THEN POKE I%*160+J%*2,219 ELSE POKE I%*160+J%*2,255
120 POKE I%*160+J%*2+1,10
140 NEXT J%
150 NEXT I%
160 FOR I%=0 TO 24
170 FOR J%=0 TO 79
180 B=PEEK(I%*160+J%*2)
190 IF B=219 THEN A(I%,J%)=219 ELSE A(I%,J%)=0
210 NEXT J%
211 NEXT I%
215 'INSERT CODE HERE FOR CELLULAR AUTOMATA RULES
220 FOR I%=1 TO 23
230 FOR J%=1 TO 78
260 C=A(I%,J%-1)
270 C=C+A(I%-1,J%)+A(I%+1,J%)
280 C=C+A(I%,J%+1)
292 IF C=219 OR C=219*3 THEN POKE I%*160+J%*2,219 ELSE POKE I%*160+J%*2,255
300 POKE I%*160+J%*2+1,10
400 NEXT J%
401 NEXT I%
402 CYCLE = CYCLE + 1
450 GOTO 160
1000 END