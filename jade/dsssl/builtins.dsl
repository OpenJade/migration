;; clause 10.1.1
(define (current-root) (node-property 'grove-root (current-node)))

;; clause 10.2.2
(define (node-list-reduce nl combine init)
   (if (node-list-empty? nl)
       init
       (node-list-reduce (node-list-rest nl)
                         combine
                         (combine init (node-list-first nl)))))

(define (node-list-contains? nl snl)
  (node-list-reduce nl
		    (lambda (result i)
		      (or result
			  (node-list=? snl i)))
		    #f))

(define (node-list-remove-duplicates nl)
  (node-list-reduce nl
		    (lambda (result snl)
		      (if (node-list-contains? result snl)
			  result
			  (node-list result snl)))
		    (empty-node-list)))

(define (reduce list combine init)
  (let loop ((result init)
	     (list list))
    (if (null? list)
	result
	(loop (combine result (car list))
	      (cdr list)))))

(define (node-list-union #!rest args)
  (reduce args
	  (lambda (nl1 nl2)
	    (node-list-reduce nl2
			      (lambda (result snl)
				(if (node-list-contains? result
							 snl)
				    result
				    (node-list result snl)))
			      nl1))
	  (empty-node-list)))

(define (node-list-intersection #!rest args)
  (if (null? args) 
      (empty-node-list)
      (reduce (cdr args)
	      (lambda (nl1 nl2)
		(node-list-reduce nl1
				  (lambda (result snl)
				    (if (node-list-contains? nl2 snl)
					(node-list result snl)
					result))
				  (empty-node-list)))
	      (node-list-remove-duplicates (car args)))))

(define (node-list-difference #!rest args)
  (if (null? args)
      (empty-node-list)
      (reduce (cdr args)
	      (lambda (nl1 nl2)
		(node-list-reduce nl1
				  (lambda (result snl)
				    (if (node-list-contains? nl2 snl)
					result
					(node-list result snl)))
				  (empty-node-list)))
	      (node-list-remove-duplicates (car args)))))

(define (node-list-symmetric-difference #!rest args)
  (if (null? args)
      (empty-node-list)
      (reduce (cdr args)
	      (lambda (nl1 nl2)
		(node-list-difference (node-list-union nl1 nl2)
				      (node-list-intersection nl1 nl2)))
	      (node-list-remove-duplicates (car args)))))

(define (node-list-union-map proc nl)
  (node-list-reduce nl
		    (lambda (result snl)
		      (node-list-union (proc snl)
				       result))
		    (empty-node-list)))

(define (node-list-some? proc nl)
  (node-list-reduce nl
		    (lambda (result snl)
		      (if (or result (proc snl))
			  #t
			  #f))
		    #f))

(define (node-list-every? proc nl)
  (node-list-reduce nl
		    (lambda (result snl)
		      (if (and result (proc snl))
			  #t
			  #f))
		    #t))

(define (node-list-filter proc nl)
  (node-list-reduce nl
		    (lambda (result snl)
		      (if (proc snl)
			  (node-list result snl)
			  result))
		    (empty-node-list)))

(define (node-list->list nl)
  (reverse (node-list-reduce nl
			     (lambda (result snl)
			       (cons snl result))
			     '())))

(define (node-list-tail nl k)
  (cond 
   ((< k 0) (empty-node-list))
   ((zero? k) nl)
   (else
    (node-list-tail (node-list-rest nl) (- k 1)))))

(define (node-list-head nl k)
  (if (zero? k)
      (empty-node-list)
      (node-list (node-list-first nl)
		 (node-list-head (node-list-rest nl) (- k 1)))))
       ;;                         ^^^^^^^
       ;;                         missing in standard

(define (node-list-sublist nl i j)
  (node-list-head (node-list-tail nl i) (- j i)))

(define (node-list-count nl)
  (node-list-length (node-list-remove-duplicates nl)))

(define (node-list-last nl)
  (node-list-ref nl 
		 (- (node-list-length nl) 1)))

;; clause 10.2.3
(define (node-list-property prop nl)
  (node-list-map (lambda (snl)
		   (node-property prop snl default: (empty-node-list)))
		 nl))

(define (origin nl)
  (node-list-property 'origin nl))

(define (origin-to-subnode-rel snl)
  (node-property 'origin-to-subnode-rel-property-name snl default: #f))

(define (tree-root nl)
  (node-list-property 'tree-root nl))

(define (grove-root nl)
  (node-list-property 'grove-root nl))

(define (source nl)
  (node-list-property 'source nl))

(define (subtree nl)
  (node-list-map (lambda (snl)
		   (node-list snl (subtree (children snl))))
		 nl))

(define (subgrove nl)
  (node-list-map
   (lambda (snl)
     (node-list snl
		(subgrove 
		 (apply node-list
			(map (lambda (name)
			       (node-property name snl))
			     (node-property 'subnode-property-names 
					    snl))))))
   nl))

(define (ancestors nl)
  (node-list-map (lambda (snl)
		   (let loop ((cur (parent snl))
			      (result (empty-node-list)))
		     (if (node-list-empty? cur)
			 result
			 (loop (parent snl)
			       (node-list cur result)))))
		 nl))

(define (grove-root-path nl)
  (node-list-map (lambda (snl)
		   (let loop ((cur (origin snl))
			      (result (empty-node-list)))
		     (if (node-list-empty? cur)
			 result
			 (loop (origin nl)
			       (node-list cur result)))))
		 nl))

(define (rsiblings nl)
  (node-list-map (lambda (snl)
		   (let ((rel (origin-to-subnode-rel snl)))
		     (if rel 
			 (node-property rel 
					(origin snl)
					default: (empty-node-list))
			 snl)))
		 nl))

(define (ipreced nl)
   (node-list-map (lambda (snl)
                  (let loop ((prev (empty-node-list))
                             (rest (siblings snl)))
                    (cond ((node-list-empty? rest)
                           (empty-node-list))
                          ((node-list=? (node-list-first rest) snl)
                           prev)
                          (else
                           (loop (node-list-first rest)
                                 (node-list-rest rest))))))
                  nl))

(define (ifollow nl)
  (node-list-map (lambda (snl)
		   (let loop ((rest (siblings snl)))
		     (cond ((node-list-empty? rest)
			    (empty-node-list))
			   ((node-list=? (node-list-first rest) snl)
			    (node-list-first (node-list-rest rest)))
			   (else
			    (loop (node-list-rest rest))))))
		 nl))

(define (grove-before? snl1 snl2)
  (let ((sorted
	 (node-list-intersection (subgrove (grove-root snl1))
				 (node-list snl1 snl2))))
    (and (= (node-list-length sorted) 2)
	 (node-list=? (node-list-first sorted) snl1))))

(define (sort-in-tree-order nl)
  (node-list-intersection (subtree (tree-root nl))
			  nl))

(define (tree-before? snl1 snl2)
  (let ((sorted 
	 (sort-in-tree-order (node-list snl1 snl2))))
    (and (= (node-list-length sorted) 2)
	 (node-list=? (node-list-first sorted) snl1))))

(define (tree-before nl)
  (node-list-map (lambda (snl)
		   (node-list-filter (lambda (x)
				       (tree-before? x snl))
				     (subtree (tree-root snl))))
		 nl))

(define (property-lookup prop snl if-present if-not-present)
  (let ((val (node-property prop snl default: #f)))
    (cond
     (val (if-present val))
     ((node-property prop snl default: #t) (if-not-present val))
     (else (if-present val)))))

(define (select-by-property nl prop proc)
  (node-list-filter (lambda (snl)
		      (let ((val (node-property prop snl default: #f)))
			(and (not (node-list? val))
			     (proc val))))
		    nl))

(define (select-by-null-property nl prop)
  (node-list-filter (lambda (snl)
		      (let ((val1 (node-property prop snl null: #f))
			    (val2 (node-property prop snl null: #t)))
			(and (not val1) val2)))
		    nl))

(define (select-by-missing-property nl prop)
  (node-list-filter (lambda (snl)
		      (let ((val1 (node-property prop snl 
						 default: #f 
						 null: #t))
			    (val2 (node-property prop snl 
						 default: #t 
						 null: #f)))
			(and (not val1) val2)))
		    nl))

;; clause 10.2.5
(define (attribute string nl)
  (node-list-map (lambda (snl)
		   (named-node name (attributes snl)))
		 nl))

(define (referent nl)
  (node-list-property 'referent nl))
  
(define (q-element pattern #!optional (nl (current-node)))
  (select-elements (subgrove nl) pattern))

(define (q-class sym #!optional (nl (current-node)))
  (node-list-filter (lambda (snl) 
		      (equal? (node-property 'class-name snl) sym)) 
		    (subgrove nl)))

(define (q-sdata string #!optional (nl (current-node)))
  (node-list-filter (lambda (snl) 
		      (and (equal? (node-property 'class-name snl) 'sdata)
			   (equal? (node-property 'system-data snl) string)))
		    (subgrove nl)))
