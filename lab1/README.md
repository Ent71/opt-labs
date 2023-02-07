1. \<signal-program> --> \<program>

2. \<program> --> PROGRAM \<procedure-identifier> ;
              <br/>\<block>.


3. \<block> --> \<variable-declarations> BEGIN
            <br/>\<statements-list> END

4. \<variable-declarations> --> VAR \<declarations-list> |
                                  <br/>\<empty>

5. \<declarations-list> --> \<declaration>
                        <br/>\<declarations-list> |
                        <br/>\<empty>

6. \<declaration> --> \<variable-identifier>:\<attribute> ;

7. \<attribute> --> INTEGER |
                <br/>FLOAT

8. \<statements-list> --> \<statement> \<statementslist> |
                      <br/>\<empty>

9. \<statement> --> \<condition-statement> ENDIF ;

10. \<condition-statement> --> \<incompletecondition-statement>\<alternative-part>

11. \<incomplete-condition-statement> --> IF
                                      <br/>\<conditional-expression> THEN
                                      <br/>\<statements-list>

12. \<alternative-part> --> ELSE \<statements-list> |
                        <br/>\<empty>

13. \<conditional-expression> --> \<expression> = \<expression>

14. \<expression> --> \<variable-identifier> |
                  <br/>\<unsigned-integer>

15. \<variable-identifier> --> \<identifier>

16. \<procedure-identifier> --> \<identifier>

17. \<identifier> --> \<letter>\<string>

18. \<string> --> \<letter>\<string> |
              <br/>\<digit>\<string> |
              <br/>\<empty>

19. \<unsigned-integer> --> \<digit>\<digits-string>

20. \<digits-string> --> \<digit>\<digits-string> |
                     <br/>\<empty>

21. \<digit> --> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

22. \<letter> --> A | B | C | D | ... | Z

![](graph.png)
