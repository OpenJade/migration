<!doctype style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN">
<!-- Copyright (C) 1999 Avi Kivity -->
                 
<![cdata[                
                 
(root
    (test-flow-object-classes)
)

(define (test-flow-object-classes)
    (make simple-page-sequence
        (make paragraph (literal "A paragraph."))
        (make side-by-side
            side-by-side-overlap-control: 'none
            (let loop
                (                          
                    (n 1)
                    (max 3)
                    (result (empty-sosofo))
                )
                (if (> n max)
                    result
                    (loop 
                        (+ n 1) 
                        max 
                        (sosofo-append
                            result
                            (make side-by-side-item
                                (make paragraph
                                    (literal 
                                        "side-by-side-item #"
                                        (format-number n "1")
                                    )
                                )
                            )
                        )
                    )
                )
            )
        )
        (make paragraph (literal "A paragraph."))
    )
)

;(declare-default-language (language "EN" "US"))

(define language
  (external-procedure "UNREGISTERED::OpenJade//Procedure::language"))

]]>
