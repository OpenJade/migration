<!DOCTYPE style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN">

<style-specification id=test1 use=test2>

(define debug (external-procedure "UNREGISTERED::James Clark//Procedure::debug"))

(root 
  (make simple-page-sequence (process-children)))

(element test (with-mode test-mode (process-children)))


;; test specificity rules

; this was necessary to avoid bugs in subgrove
;(define (doc-root) (node-property 'docelem (current-root))) 
(define doc-root current-root)

(mode test-mode

(query 
  (q-element 'A (doc-root))
  (make paragraph (literal "query (priority 1) matching " (gi (current-node))))
  1)
  
(query 
  (node-list 
    (q-element 'A (doc-root)) 
    (q-element 'B (doc-root)))
  (make paragraph (literal "query (priority 0) matching " (gi (current-node)))))
  
(query 
  (node-list 
    (q-element 'A (doc-root)) 
    (q-element 'B (doc-root)) 
    (q-element 'C (doc-root))) 
  (make paragraph (literal "query (priority -1) matching " (gi (current-node))))
  -1)
  
(id A (make paragraph (literal "id matching " (gi (current-node)))))
(id B (make paragraph (literal "id matching " (gi (current-node)))))
(id C (make paragraph (literal "id matching " (gi (current-node)))))
(id D (make paragraph (literal "id matching " (gi (current-node)))))

(element A (make paragraph (literal "element matching " (gi (current-node)))))
(element B (make paragraph (literal "element matching " (gi (current-node)))))
(element C (make paragraph (literal "element matching " (gi (current-node)))))
(element D (make paragraph (literal "element matching " (gi (current-node)))))
(element E (make paragraph (literal "element matching " (gi (current-node)))))

) ; end of test mode

(query 
  (node-list 
    (q-element 'A (doc-root)) 
    (q-element 'B (doc-root)) 
    (q-element 'C (doc-root)) 
    (q-element 'D (doc-root)) 
    (q-element 'E (doc-root)) 
    (q-element 'F (doc-root))) 
  (make paragraph (literal "initial mode query (priority 1) matching " (gi (current-node))))
  1)
  

(query 
  (node-list 
    (q-element 'A (doc-root)) 
    (q-element 'B (doc-root)) 
    (q-element 'C (doc-root)) 
    (q-element 'D (doc-root)) 
    (q-element 'E (doc-root)) 
    (q-element 'F (doc-root)) 
    (q-element 'G (doc-root))) 
  (make paragraph (literal "initial mode query (priority 0) matching " (gi (current-node)))))
  
(query 
  (node-list 
    (q-element 'A (doc-root)) 
    (q-element 'B (doc-root)) 
    (q-element 'C (doc-root)) 
    (q-element 'D (doc-root)) 
    (q-element 'E (doc-root)) 
    (q-element 'F (doc-root)) 
    (q-element 'G (doc-root)) 
    (q-element 'H (doc-root))) 
  (make paragraph (literal "initial mode query (priority -1) matching " (gi (current-node))))
  -1)
  
(id A (make paragraph (literal "initial mode id matching " (gi (current-node)))))
(id B (make paragraph (literal "initial mode id matching " (gi (current-node)))))
(id C (make paragraph (literal "initial mode id matching " (gi (current-node)))))
(id D (make paragraph (literal "initial mode id matching " (gi (current-node)))))
(id E (make paragraph (literal "initial mode id matching " (gi (current-node)))))
(id F (make paragraph (literal "initial mode id matching " (gi (current-node)))))
(id G (make paragraph (literal "initial mode id matching " (gi (current-node)))))
(id H (make paragraph (literal "initial mode id matching " (gi (current-node)))))
(id I (make paragraph (literal "initial mode id matching " (gi (current-node)))))

(element A (make paragraph (literal "initial mode element matching " (gi (current-node)))))
(element B (make paragraph (literal "initial mode element matching " (gi (current-node)))))
(element C (make paragraph (literal "initial mode element matching " (gi (current-node)))))
(element D (make paragraph (literal "initial mode element matching " (gi (current-node)))))
(element E (make paragraph (literal "initial mode element matching " (gi (current-node)))))
(element F (make paragraph (literal "initial mode element matching " (gi (current-node)))))
(element G (make paragraph (literal "initial mode element matching " (gi (current-node)))))
(element H (make paragraph (literal "initial mode element matching " (gi (current-node)))))
(element I (make paragraph (literal "initial mode element matching " (gi (current-node)))))
(element J (make paragraph (literal "initial mode element matching " (gi (current-node)))))

<style-specification id=test2>

(query 
  (node-list 
    (q-element 'A (doc-root)) 
    (q-element 'B (doc-root)) 
    (q-element 'C (doc-root)) 
    (q-element 'D (doc-root)) 
    (q-element 'E (doc-root)) 
    (q-element 'F (doc-root)) 
    (q-element 'G (doc-root)) 
    (q-element 'H (doc-root)) 
    (q-element 'I (doc-root)) 
    (q-element 'J (doc-root)) 
    (q-element 'K (doc-root))) 
  (make paragraph (literal "part 2 initial mode query (priority 1) matching " (gi (current-node))))
  1)
  

(query 
  (node-list 
    (q-element 'A (doc-root)) 
    (q-element 'B (doc-root)) 
    (q-element 'C (doc-root)) 
    (q-element 'D (doc-root)) 
    (q-element 'E (doc-root)) 
    (q-element 'F (doc-root)) 
    (q-element 'G (doc-root)) 
    (q-element 'H (doc-root)) 
    (q-element 'I (doc-root)) 
    (q-element 'J (doc-root)) 
    (q-element 'K (doc-root)) 
    (q-element 'L (doc-root))) 
  (make paragraph (literal "part 2 initial mode query (priority 0) matching " (gi (current-node)))))
  
(query 
  (node-list 
    (q-element 'A (doc-root)) 
    (q-element 'B (doc-root)) 
    (q-element 'C (doc-root)) 
    (q-element 'D (doc-root)) 
    (q-element 'E (doc-root)) 
    (q-element 'F (doc-root)) 
    (q-element 'G (doc-root)) 
    (q-element 'H (doc-root)) 
    (q-element 'I (doc-root)) 
    (q-element 'J (doc-root)) 
    (q-element 'K (doc-root)) 
    (q-element 'L (doc-root)) 
    (q-element 'M (doc-root))) 
  (make paragraph (literal "part 2 initial mode query (priority -1) matching " (gi (current-node))))
  -1)
  
(id A (make paragraph (literal "part 2 initial mode id matching " (gi (current-node)))))
(id B (make paragraph (literal "part 2 initial mode id matching " (gi (current-node)))))
(id C (make paragraph (literal "part 2 initial mode id matching " (gi (current-node)))))
(id D (make paragraph (literal "part 2 initial mode id matching " (gi (current-node)))))
(id E (make paragraph (literal "part 2 initial mode id matching " (gi (current-node)))))
(id F (make paragraph (literal "part 2 initial mode id matching " (gi (current-node)))))
(id G (make paragraph (literal "part 2 initial mode id matching " (gi (current-node)))))
(id H (make paragraph (literal "part 2 initial mode id matching " (gi (current-node)))))
(id I (make paragraph (literal "part 2 initial mode id matching " (gi (current-node)))))
(id J (make paragraph (literal "part 2 initial mode id matching " (gi (current-node)))))
(id K (make paragraph (literal "part 2 initial mode id matching " (gi (current-node)))))
(id L (make paragraph (literal "part 2 initial mode id matching " (gi (current-node)))))
(id M (make paragraph (literal "part 2 initial mode id matching " (gi (current-node)))))
(id N (make paragraph (literal "part 2 initial mode id matching " (gi (current-node)))))

(element A (make paragraph (literal "part 2 initial mode element matching " (gi (current-node)))))
(element B (make paragraph (literal "part 2 initial mode element matching " (gi (current-node)))))
(element C (make paragraph (literal "part 2 initial mode element matching " (gi (current-node)))))
(element D (make paragraph (literal "part 2 initial mode element matching " (gi (current-node)))))
(element E (make paragraph (literal "part 2 initial mode element matching " (gi (current-node)))))
(element F (make paragraph (literal "part 2 initial mode element matching " (gi (current-node)))))
(element G (make paragraph (literal "part 2 initial mode element matching " (gi (current-node)))))
(element H (make paragraph (literal "part 2 initial mode element matching " (gi (current-node)))))
(element I (make paragraph (literal "part 2 initial mode element matching " (gi (current-node)))))
(element J (make paragraph (literal "part 2 initial mode element matching " (gi (current-node)))))
(element K (make paragraph (literal "part 2 initial mode element matching " (gi (current-node)))))
(element L (make paragraph (literal "part 2 initial mode element matching " (gi (current-node)))))
(element M (make paragraph (literal "part 2 initial mode element matching " (gi (current-node)))))
(element N (make paragraph (literal "part 2 initial mode element matching " (gi (current-node)))))
(element O (make paragraph (literal "part 2 initial mode element matching " (gi (current-node)))))

(default (make paragraph (literal "part 2 initial mode default matching " (gi (current-node)))))

