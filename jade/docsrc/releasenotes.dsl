<!DOCTYPE style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN" [
<!ENTITY html-ss 
  PUBLIC "-//Norman Walsh//DOCUMENT DocBook HTML Stylesheet//EN" CDATA dsssl>
<!ENTITY print-ss
  PUBLIC "-//Norman Walsh//DOCUMENT DocBook Print Stylesheet//EN" CDATA dsssl>
]>
<style-sheet>
<style-specification id="print" use="print-stylesheet">
<style-specification-body> 

;; customize the print stylesheet

(define %chapter-autolabel%
  ;; Are chapters enumerated?
  #f)

(define %two-side% #t)

(define %hsize-bump-factor% 1.17 )

</style-specification-body>
</style-specification>
<style-specification id="html" use="html-stylesheet">
<style-specification-body> 

;; customize the html stylesheet
(define %root-filename%
  ;; Name for the root HTML document
  "releasenotes" )
(define %html-ext% ".html")
(define %body-attr%
  ;; What attributes should be hung off of BODY?
  (list
     (list "BGCOLOR" "#FFFFFF")
     (list "TEXT" "#000000")))

</style-specification-body>
</style-specification>
<external-specification id="print-stylesheet" document="print-ss">
<external-specification id="html-stylesheet"  document="html-ss">
</style-sheet>
