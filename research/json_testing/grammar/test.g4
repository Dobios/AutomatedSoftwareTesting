grammar arr;

valuev
   : 'true'
   | 'false'
   | 'null'
   ;

value
   : valuev
   ;

arr
   : '[' value ',' value ',' value ']'
   | '[' ']'
   ;