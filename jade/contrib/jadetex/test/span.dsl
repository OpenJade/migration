<!doctype style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN"[]>
<style-specification>

;; the command line variable 'selection' determines
;; which three routines are applied to the first three parts
;; (to be precise: to the first three children of the top level element)
;; of the sgml file;
;; for a description of the routines see below

;; default selection
(define selection "1")

(define routines
  (case (string->number selection)
	;; display groups
	;; span 1, 2, 1
	((1) (list dgone dgtwo dgone))
	;; span 2, 1, 2
	((2) (list dgtwo dgone dgtwo))
	;; span 2, 2, 1
	((3) (list dgtwo dgtwo dgone))
	;; span 1, 1, 2
	((4) (list dgone dgone dgtwo))
	;; paragraphs
	;; span 1, 2, 1
	((5) (list pgone pgtwo pgone))
	;; span 2, 1, 2
	((6) (list pgtwo pgone pgtwo))
	;; span 2, 2, 1
	((7) (list pgtwo pgtwo pgone))
	;; span 1, 1, 2
	((8) (list pgone pgone pgtwo))
	;; nested display groups
	;; two parts in dg with span 2, pg with span 1
	((9) (list twoindgtwo none pgone))
	;; tow parts in dg with span 1, pg with span 1
	((10) (list twoindgone none pgone))
	;; pg with span 1, two parts in dg with span 2
	((11) (list pgone twoindgtwo none))
	))

;; the routines which are applied to a part;
;; the display-groups and paragraphs made by these routines
;; are indentified by a rule at the top;

;; node in a display group with span 2
(define (dgtwo node)
  (make display-group
	span: 2
	(make rule)
	(process-children-trim)))

;; node in a display group
(define (dgone node)
  (make display-group
	(make rule)
	(process-children-trim)))

;; node in a paragraph with span 2
(define (pgtwo node)
  (make paragraph
	span: 2
	(make rule)
	(process-children-trim)))

;; node in a paragraph
(define (pgone node)
  (make display-group
	(make rule)
	(process-children-trim)))

;; next two nodes in a display group
;; node 1 in dgtwo (with span 2), but span is not listened to
;; node 2 in dgone
(define (twoindgone node)
  (make display-group
	(make rule
	  line-thickness: 2pt)
	(dgtwo node)
	(dgone (ifollow node))))

;; next two nodes in a display group with span 2
;; node 1 in dgtwo (with span 2), but span is not listened to
;; node 2 in dgone
(define (twoindgtwo node)
  (make display-group
	span: 2
	(make rule
	  line-thickness: 2pt)
	(dgtwo node)
	(dgone (ifollow node))))

;; skip this node
(define (none node)
  (empty-sosofo))

(declare-characteristic page-n-columns
      "UNREGISTERED::James Clark//Characteristic::page-n-columns" 1)
(declare-characteristic page-column-sep
      "UNREGISTERED::James Clark//Characteristic::page-column-sep" 4pt)

(define-unit pc (/ 1in 6))

(root
	(make simple-page-sequence
	  left-margin: 5pc
	  top-margin: 5pc
	  bottom-margin: 5pc
	  page-width: 30pc
	  page-n-columns: 2
	  page-column-sep: 1pc
		(do-parts)))

(define (do-parts #!optional (nl (children (current-node))))	 
  (let loop ((ndl nl) (part 0) (sosofo (empty-sosofo)))
	(case part
	 ((0 1)
	  (loop (node-list-rest ndl)
			(+ part 1)
			(sosofo-append sosofo
						   ((list-ref routines part) (node-list-first ndl)))))
	 ((2)
	  (sosofo-append sosofo
					 ((list-ref routines part) (node-list-first ndl))))
	 (else sosofo)
	 )))

(element p
  (make paragraph
	(process-children-trim)))

</style-specification>
