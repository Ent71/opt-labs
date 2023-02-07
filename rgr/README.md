1. \<signal-program> --> \<program>  

2. \<program> --> PROGRAM \<procedure-identifier> ;  
              \<block>.  
 
3. \<block> --> \<variable-declarations> BEGIN  
            \<statements-list> END  

4. \<variable-declarations> --> VAR \<declarations-list> |  
                                   \<empty>  

5. \<declarations-list> --> \<declaration>  
                        \<declarations-list> |  
                        \<empty>  

6. \<declaration> --> <variable-identifier>:<attribute> ;  

7. \<attribute> --> INTEGER |  
                FLOAT  

8. \<statements-list> --> \<statement> \<statementslist> | 
                      \<empty>  

9. \<statement> --> \<condition-statement> ENDIF ;  

10. \<condition-statement> --> \<incompletecondition-statement>\<alternative-part> 


11. \<incomplete-condition-statement> --> IF  
                                      \<conditional-expression> THEN  
                                      \<statements-list>  

12. \<alternative-part> --> ELSE \<statements-list> |  
                        \<empty>  

13. \<conditional-expression> --> \<expression> = \<expression>  

14. \<expression> --> \<variable-identifier> | 
                  \<unsigned-integer>  

15. \<variable-identifier> --> \<identifier>  

16. \<procedure-identifier> --> \<identifier>  

17. \<identifier> --> \<letter><string> 

18. \<string> --> \<letter>\<string> |  
              \<digit>\<string> |  
              \<empty>  

19. \<unsigned-integer> --> \<digit>\<digits-string>  

20. \<digits-string> --> \<digit><digits-string> |  
                     \<empty>  

21. \<digit> --> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9  

22. \<letter> --> A | B | C | D | ... | Z  

|           |     Адреса операції                      |     Код      операції                    |     АТ     (Адреса True)                                             |     AF     (Адреса False)                      |
|-----------|------------------------------------------|------------------------------------------|----------------------------------------------------------------------|------------------------------------------------|
|     1     |     \<signal-program>                    |     \<program>                           |     2                                                                |     F                                          |
|     2     |     \<program>                           |     PROGRAM                              |     3                                                                |     F                                          |
|     3     |                                          |     \<procedure-identifier>              |     4                                                                |     F                                          |
|     4     |                                          |     ;                                    |     5                                                                |     F                                          |
|     5     |                                          |     <block>                              |     6                                                                |     F                                          |
|     6     |                                          |     .                                    |     T                                                                |     F                                          |
|     7     |     \<block>                             |     \<variable-declarations>             |     8                                                                |     F                                          |
|     8     |                                          |     BEGIN                                |     9                                                                |     F                                          |
|     9     |                                          |     \<statements-list>                   |     10                                                               |     F                                          |
|     10    |                                          |     END                                  |     T                                                                |     F                                          |
|     11    |     \<variable-declarations>             |     VAR                                  |     if (lexem == VAR) goto 12       else return T                    |     Якщо знизу повернули F, то повертаємо F    |
|     12    |                                          |     <declarations-list>                  |     13                                                               |     F                                          |
|     13    |     \<declarations-list>                 |     \<declaration>                       |     if(lexem==<identifier>)    goto 15     else return T             |     Якщо знизу повернули F, то повертаємо F    |
|     14    |                                          |     \<declarations-list>                 |     13                                                               |     F                                          |
|     15    |     \<declaration>                       |     \<variable-identifier>               |     16                                                               |     F                                          |
|     16    |                                          |     :                                    |     17                                                               |     F                                          |
|     17    |                                          |     \<attribute>                         |     18                                                               |     F                                          |
|     18    |                                          |     ;                                    |     T                                                                |     F                                          |
|     19    |     \<attribute>                         |     INTEGER                              |     if (lexem == INTEGER) return T     else goto 20                  |     Якщо знизу повернули F, то повертаємо F    |
|     20    |                                          |     FLOAT                                |     T                                                                |     F                                          |
|     21    |     \<statements-list>                   |     \<statement>                         |     if(lexem==IF ) goto 23     else return T                         |     F                                          |
|     22    |                                          |     \<statements-list>                   |     T                                                                |     F                                          |
|     23    |     \<statement>                         |     \<condition-statement>               |     24                                                               |     F                                          |
|     24    |                                          |     ENDIF                                |     25                                                               |     F                                          |
|     25    |                                          |     ;                                    |     T                                                                |     F                                          |
|     26    |     \<condition-statement>               |     \<incomplete-condition-statement>    |     27                                                               |     F                                          |
|     27    |                                          |     \<alternative-part>                  |     T                                                                |     F                                          |
|     28    |     \<incomplete-condition-statement>    |     IF                                   |     29                                                               |     F                                          |
|     29    |                                          |     \<conditional-expression>            |     30                                                               |     F                                          |
|     30    |                                          |     THEN                                 |     31                                                               |     F                                          |
|     31    |                                          |     \<statements-list>                   |     T                                                                |     F                                          |
|     32    |     \<alternative-part>                  |     ELSE                                 |     if (lexem == ELSE) goto 33     else return true                  |     Якщо знизу повернули F, то повертаємо F    |
|     33    |                                          |     \<statements-list>                   |     T                                                                |     F                                          |
|     34    |     \<conditional-expression>            |     \<expression>                        |     35                                                               |     F                                          |
|     35    |                                          |     =                                    |     36                                                               |     F                                          |
|     36    |                                          |     \<expression>                        |     T                                                                |     F                                          |
|     37    |     \<expression>                        |     \<variable-identifier>               |     if (lexem == <variable-identifier>) return T     else goto 38    |     Якщо знизу повернули F, то повертаємо F    |
|     38    |                                          |     \<unsigned-integer>                  |     T                                                                |     F                                          |
|     39    |     \<variable-identifier>               |     \<identifier>                        |     T                                                                |     F                                          |
|     40    |     \<procedure-identifier>              |     \<identifier>                        |     T                                                                |     F                                          |