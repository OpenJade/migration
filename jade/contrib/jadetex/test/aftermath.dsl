<!doctype style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN"[]>
<style-specification>

(define-unit pc (/ 1in 6))
(root
	(make simple-page-sequence
	  left-margin: 5pc
	  top-margin: 5pc
	  page-width: 15pc
	  first-line-start-indent: 1pc
	  (make paragraph
		(literal "Test of indentation after a math sequence:")
		(make math-sequence
		  math-display-mode: 'display
		  (literal "A = B"))
		(literal "Some text after the math sequence"))
	  (make paragraph
		(literal "Test of indentation after a math sequence:")
		(make display-group
		  space-before: .5pc
		  space-after: .5pc
		  (make math-sequence
			math-display-mode: 'display
			(literal "A = B")))
		(literal "Some text after the math sequence"))
	  (make paragraph
		(literal "And a final paragraph."))
	  ))

</style-specification>

