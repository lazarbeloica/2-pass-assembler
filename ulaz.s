--------------------------TABELA SIMBOLA---------------------------------------

r.br.     ime                 sekcija             offset    doseg   
0         <UND>               <UND>               0          local  
1         .data.prvi          .data.prvi          0          local  
2         d                   .data.prvi          4          local  
3         .text.prvi          .text.prvi          0          local  
4         a                   .text.prvi          0          global 
5         b                   .text.prvi          4          global 
6         .text.drugi         .text.drugi         0          local  
7         e                   .text.drugi         4          local  
8         potprogram          .text.drugi         18         local  
9         c                   <UND>               0          global 


--------------------------TABELA SEKCIJA---------------------------------------

.data.prvi

 0 :  F2 00 00 0A 
 4 :  05 00 05 00 
 8 :  00 00 00 f9 
 c :  ff ff ff fa 
10 :  ff ff ff 

.rel.data.prvi

sim.    ofset     tip       
3       7         R_32_RELADD
3       b         R_32_RELADD

------------------------------------------
.text.drugi

 0 :  EC 18 00 80 
 4 :  ED 00 80 13 
 8 :  EE 00 00 19 
 c :  8E 00 92 08 
10 :  EC F4 FF 87 
14 :  EC F0 FF 87 
18 :  EE 00 C0 08 


.rel.text.drugi

sim.    ofset     tip       
3       14        R_32_RELADD

------------------------------------------
.text.prvi

 0 :  E1 00 40 08 
 4 :  11 00 00 0A 
 8 :  EC FF FF 0F 
 c :  EC F8 FF 87 
10 :  EA 04 0C 0D 
14 :  EF F0 FF 28 
18 :  EF EC FF 20 


.rel.text.prvi

sim.    ofset     tip       
1       14        R_32_RELADD
1       18        R_32_RELADD

------------------------------------------
<UND>



.rel<UND>

sim.    ofset     tip       

------------------------------------------

