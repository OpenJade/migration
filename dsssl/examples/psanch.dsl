<!doctype style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN">

<style-sheet>
<style-specification>
<style-specification-body>


(element prova 
  (make page-sequence
   force-last-page: 'front
   justify-spread?: #t 
   (make column-set-sequence 
     position-preference: 'bottom
     span: 2
    (make anchor 
      break-before-priority: 2 
      display?: #t
      anchor-keep-with-previous?: #t))))



</style-specification-body>
</style-specification>
</style-sheet>
