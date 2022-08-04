(module interp (lib "eopl.ss" "eopl")
  
  ;; interpreter for the PROC language, using the procedural
  ;; representation of procedures.

  ;; The \commentboxes are the latex code for inserting the rules into
  ;; the code in the book. These are too complicated to put here, see
  ;; the text, sorry. 

  (require "drscheme-init.scm")

  (require "lang.scm")
  (require "data-structures.scm")
  (require "environments.scm")

  (provide value-of-program value-of)

;;;;;;;;;;;;;;;; the interpreter ;;;;;;;;;;;;;;;;

  ;; value-of-program : Program -> ExpVal
  (define value-of-program 
    (lambda (pgm)
      (cases program pgm
        (a-program (exp1)
          (value-of exp1 (init-env))))))

  ;; value-of : Exp * Env -> ExpVal
  (define value-of
    (lambda (exp env)
      (cases expression exp

        ;\commentbox{ (value-of (const-exp \n{}) \r) = \n{}}
        (const-exp (num) (num-val num))

        ;\commentbox{ (value-of (var-exp \x{}) \r) = (apply-env \r \x{})}
        (var-exp (var) (apply-env env var))

        ;\commentbox{\diffspec}
        (diff-exp (exp1 exp2)
          (let ((val1 (value-of exp1 env))
                (val2 (value-of exp2 env)))
            (let ((num1 (expval->num val1))
                  (num2 (expval->num val2)))
              (num-val
                (- num1 num2)))))

        ;\commentbox{\zerotestspec}
        (zero?-exp (exp1)
          (let ((val1 (value-of exp1 env)))
            (let ((num1 (expval->num val1)))
              (if (zero? num1)
                (bool-val #t)
                (bool-val #f)))))
              
        ;\commentbox{\ma{\theifspec}}
        (if-exp (exp1 exp2 exp3)
          (let ((val1 (value-of exp1 env)))
            (if (expval->bool val1)
              (value-of exp2 env)
              (value-of exp3 env))))

        ;\commentbox{\ma{\theletspecsplit}}
        (let-exp (var exp1 body)       
          (let ((val1 (value-of exp1 env)))
            (value-of body
              (extend-env var val1 env))))
        
        (proc-exp (var body)
          (proc-val (procedure var body env)))

        (call-exp (rator rand)
          (let ((proc (expval->proc (value-of rator env)))
                (arg (value-of rand env)))
            (apply-procedure proc arg)))
        
;        ;Question 1
;-----------------------------------------------------------------------------------------------------------------
        (argmax-exp (prc args)
          (if(null? args)              
             (eopl:error "Number of arguments iz zero!!")
              (letrec ((max (expval->num(value-of (call-exp prc (car args))env)))
                    (innerArgmax (lambda (prc args max)
                                   (if (null? args)
                                        (num-val max)
                                       (let ((tempMax (expval->num(value-of (call-exp prc (car args))env))))
                                         (if(> tempMax max)
                                            (innerArgmax prc (cdr args) tempMax )
                                            (innerArgmax prc (cdr args) max )))))))
                     (innerArgmax prc (cdr args) max )
                    )))
;---------------------------------------------------------------------------------------------------------------
                                             
        
        ;Question 3
;--------------------------------------------------------------------------------------------------------------------        
        (mapsum-exp (prc args)
          (if(null? args)              
              (num-val 0)
              (innerMapsum prc (cdr args) (expval->num(value-of (call-exp prc (car args)) env )) env)))
;------------------------------------------------------------------------------------------------------------------
        )))
  


  ;; procedure : Var * Exp * Env -> Proc
  ;; Page: 79
  (define procedure
    (lambda (var body env)
      (lambda (val)
        (value-of body (extend-env var val env)))))
  
  ;; apply-procedure : Proc * ExpVal -> ExpVal
  ;; Page: 79
  (define apply-procedure
    (lambda (proc val)
      (proc val)))
  

  ;Question 3( Help Method)
 ;--------------------------------------------------------------------------------------------------- 
  ( define innerMapsum (lambda( prc args Sum env ) 
     (if(null? args)
        (num-val Sum)
        ( let (( tempSum  (+ Sum (expval->num(value-of (call-exp prc (car args)) env )) )))
         (innerMapsum prc (cdr args)  tempSum env )))))
;----------------------------------------------------------------------------------------------------
  )
