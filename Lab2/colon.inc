 %define link 0 ;идентификатор
 
 ;макрос colon
 ;принимает два параметра:
 ;ключ
 ;имя
 %macro colon 2
	%%link: dq link ;создание ссылки на следующее слово
	db %1, 0 		;обнуление ключа
	xt_ %+ %2: 
	%define link %%link
 %endmacro
 

