<!DOCTYPE style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN">

(define (check-node nl nd)
  (make paragraph
    (literal "node " (gi nd)
	     (if (node-list-contains? nl nd) " is " " is not ")
	     "in subtree with start " (gi (node-list-first nl)))))
(root 
    (make simple-page-sequence
      (let ((nl (subtree (node-property 'docelem (current-root)))))
	(let loop ((nl1 nl) (ret (empty-sosofo)))
	  (if (node-list-empty? nl1)
	      ret
	      (let loop2 ((nl2 nl) (ret2 ret))
		(if (node-list-empty? nl2)
		    (loop (node-list-rest nl1) ret2)
		    (loop2 (node-list-rest nl2) 
			   (sosofo-append 
			    ret2 
			    (check-node nl1 (node-list-first nl2)))))))))))
