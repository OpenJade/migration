<!doctype style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN"[]>
<style-specification>

(define-unit pc (/ 1in 6))
(root
	(make simple-page-sequence
	  left-margin: 5pc
	  top-margin: 5pc
	  page-width: 15pc
	  (make paragraph
		(literal "Test of fence flow object:")
		(make math-sequence
		  math-display-mode: 'display
		  (make fence
			(literal "A = B")
			(make character
			  label: 'open
			  char: #\{)
			(make character
			  label: 'close
			  char: #\}))))))

</style-specification>
