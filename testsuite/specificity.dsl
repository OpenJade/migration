<!DOCTYPE style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN">

<style-specification id=test1 use=test2>

(define debug (external-procedure "UNREGISTERED::James Clark//Procedure::debug"))

(root 
  (make simple-page-sequence (process-children)))

(element test (with-mode test-mode (process-children)))


;; test specificity rules

(mode test-mode

(query 
  (q-element 'A (node-property 'docelem (current-root))) 
  (make paragraph (literal "query (priority 1) matching " (gi (current-node))))
  1)
  
(query 
  (node-list 
    (q-element 'A (node-property 'docelem (current-root))) 
    (q-element 'B (node-property 'docelem (current-root)))) 
  (make paragraph (literal "query (priority 0) matching " (gi (current-node)))))
  
(query 
  (node-list 
    (q-element 'A (current-root)) 
    (q-element 'B (current-root)) 
    (q-element 'C (current-root))) 
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
    (q-element 'A (current-root)) 
    (q-element 'B (current-root)) 
    (q-element 'C (current-root)) 
    (q-element 'D (current-root)) 
    (q-element 'E (current-root)) 
    (q-element 'F (current-root))) 
  (make paragraph (literal "initial mode query (priority 1) matching " (gi (current-node))))
  1)
  

(query 
  (node-list 
    (q-element 'A (current-root)) 
    (q-element 'B (current-root)) 
    (q-element 'C (current-root)) 
    (q-element 'D (current-root)) 
    (q-element 'E (current-root)) 
    (q-element 'F (current-root)) 
    (q-element 'G (current-root))) 
  (make paragraph (literal "initial mode query (priority 0) matching " (gi (current-node)))))
  
(query 
  (node-list 
    (q-element 'A (current-root)) 
    (q-element 'B (current-root)) 
    (q-element 'C (current-root)) 
    (q-element 'D (current-root)) 
    (q-element 'E (current-root)) 
    (q-element 'F (current-root)) 
    (q-element 'G (current-root)) 
    (q-element 'H (current-root))) 
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
    (q-element 'A (current-root)) 
    (q-element 'B (current-root)) 
    (q-element 'C (current-root)) 
    (q-element 'D (current-root)) 
    (q-element 'E (current-root)) 
    (q-element 'F (current-root)) 
    (q-element 'G (current-root)) 
    (q-element 'H (current-root)) 
    (q-element 'I (current-root)) 
    (q-element 'J (current-root)) 
    (q-element 'K (current-root))) 
  (make paragraph (literal "part 2 initial mode query (priority 1) matching " (gi (current-node))))
  1)
  

(query 
  (node-list 
    (q-element 'A (current-root)) 
    (q-element 'B (current-root)) 
    (q-element 'C (current-root)) 
    (q-element 'D (current-root)) 
    (q-element 'E (current-root)) 
    (q-element 'F (current-root)) 
    (q-element 'G (current-root)) 
    (q-element 'H (current-root)) 
    (q-element 'I (current-root)) 
    (q-element 'J (current-root)) 
    (q-element 'K (current-root)) 
    (q-element 'L (current-root))) 
  (make paragraph (literal "part 2 initial mode query (priority 0) matching " (gi (current-node)))))
  
(query 
  (node-list 
    (q-element 'A (current-root)) 
    (q-element 'B (current-root)) 
    (q-element 'C (current-root)) 
    (q-element 'D (current-root)) 
    (q-element 'E (current-root)) 
    (q-element 'F (current-root)) 
    (q-element 'G (current-root)) 
    (q-element 'H (current-root)) 
    (q-element 'I (current-root)) 
    (q-element 'J (current-root)) 
    (q-element 'K (current-root)) 
    (q-element 'L (current-root)) 
    (q-element 'M (current-root))) 
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

