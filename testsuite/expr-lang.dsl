<!doctype style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN">
<!-- Copyright (C) 1999 Avi Kivity -->
                 
<![cdata[                
                 
(root
    (test-expression-language)
)

(define (test-expression-language)
    (sosofo-append
        (make-test "Character Primitives" test-char)
        (make-test "String Primitives" test-string)
    )
)

(define (test-char)
    (sosofo-append
        (testcase char=?  "char=?"  #\a #\b)
        (testcase char=?  "char=?"  #\a #\a)
        (testcase char=?  "char=?"  #\a #\A)
        (testcase char<?  "char<?"  #\a #\b)
        (testcase char<?  "char<?"  #\a #\a)
        (testcase char<?  "char<?"  #\a #\A)
        (testcase char<=? "char<=?" #\a #\b)
        (testcase char<=? "char<=?" #\a #\a)
        (testcase char<=? "char<=?" #\a #\A)
        (testcase char>?  "char>?"  #\a #\b)
        (testcase char>?  "char>?"  #\a #\a)
        (testcase char>?  "char>?"  #\a #\A)
        (testcase char>=? "char>=?" #\a #\b)
        (testcase char>=? "char>=?" #\a #\a)
        (testcase char>=? "char>=?" #\a #\A)
        (testcase char-ci=?  "char-ci=?"  #\a #\b)
        (testcase char-ci=?  "char-ci=?"  #\a #\a)
        (testcase char-ci=?  "char-ci=?"  #\a #\A)
        (testcase char-ci<?  "char-ci<?"  #\a #\b)
        (testcase char-ci<?  "char-ci<?"  #\a #\a)
        (testcase char-ci<?  "char-ci<?"  #\a #\A)
        (testcase char-ci<=? "char-ci<=?" #\a #\b)
        (testcase char-ci<=? "char-ci<=?" #\a #\a)
        (testcase char-ci<=? "char-ci<=?" #\a #\A)
        (testcase char-ci>?  "char-ci>?"  #\a #\b)
        (testcase char-ci>?  "char-ci>?"  #\a #\a)
        (testcase char-ci>?  "char-ci>?"  #\a #\A)
        (testcase char-ci>=? "char-ci>=?" #\a #\b)
        (testcase char-ci>=? "char-ci>=?" #\a #\a)
        (testcase char-ci>=? "char-ci>=?" #\a #\A)
    )
)

(define (test-string)
    (empty-sosofo)
)
                  
(define (testcase proc proc-string #!rest args)
    (write-line
        (string-append
            "(" 
            proc-string
            (reduce 
                args
                (lambda (args arg) (string-append args " " (obj->string arg)))
                ""
            )
            " --> "
            (obj->string (apply proc args))
        )
    )
)       

(define (obj->string obj)
    (cond
        ((char? obj) (list->string (list #\# #\\ obj)))
        ((string? obj) (string-append "\"" obj "\""))
        ((boolean? obj) (if obj "#t" "#f"))
        (else "unimplemented")
    )
)           
                  
(define (make-test title proc)
    (sosofo-append
        (write-line title)
        (proc)
        (write-empty-line)
    )
)    

(define (write-empty-line) (write-line ""))

(define newline (list->string (list #\line-feed #\carriage-return)))

(define (write-line string)
    (make formatting-instruction
        data: (string-append string newline)
    )
)

(declare-flow-object-class formatting-instruction
  "UNREGISTERED::James Clark//Flow Object Class::formatting-instruction")

(declare-default-language (language "EN" "US"))

(define language
  (external-procedure "UNREGISTERED::OpenJade//Procedure::language"))

]]>
