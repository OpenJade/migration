<!DOCTYPE style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN" >

(root (make simple-page-sequence (process-children)))

(element A
  (with-mode test-mode
    (make sequence
      (process-node-list (q-element 'sub (sgml-parse "query-root2.sgml")))
      (process-children))))

(define (describe-match s)
  (make paragraph 
    (literal "test-mode " s " matching " (gi (current-node))
             " in " (gi (parent (current-node))))))
 
(mode test-mode

;; this should match sub elements in the grove being processed, but
;; not in node-lists created via sgml-parse
(query (q-element 'sub (current-root))
  (describe-match "query rule"))

(default 
  (describe-match "default rule"))
  
) ;; end of test-mode 
