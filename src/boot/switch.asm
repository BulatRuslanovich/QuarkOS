; Переключаемся в PM (Protected mode, защищенный режим)
; ------------------------------------------------------------------------------
;	Чтобы сделать свитч, нам нужно:
;		1. Отключить прерывания (процессор просто будет их игнорировать), т.к.
; 		в PM, прерывания обрабатываются совершенно по-другому в отличие от
;		Real Mode. Даже если процессор и смог бы распределить сигналы прерываний
;		по конкретным BIOS обработчикам прерываний, БИОС обработчики будут
;		обрабатывать 16-битный код, что повлекло бы за собой ошибки.
;		2. Загрузить GDT дескриптор
;		3. Изменяем первый бит регистра управления cr0 на "1"
;		https://en.wikipedia.org/wiki/Control_register#CR0
;		4. Т.к. процессор использует специальную технику, которая называется
;		называется pipelining (гугли: вычислительный конвейер, полезная статья
;		на хабре: https://habr.com/ru/post/182002/), и поэтому сразу после того,
;		как перевести процессор в PM (что мы и сделали в предыдущем пункте),
;		нам нужно заставить процессор завершить всю работу в конвейере, чтобы
;		быть уверенным, что все будущие инструкции будут выполнены корректно.
;		Конвейер загружает в себя некоторые количество последующих после текущей
;		инструкций, но конвейеру не очень нравятся инструкции типа call и jmp,
;		т.к. процессор не знает полностью какие инструкции будут следовать за
;		ними, в особенности если мы вызовем jmp или call "прыгая" в другой
;		сегмент. Поэтому нам нужно сделать "дальний прыжок", чтобы завершить
;		обрабатываемые в конвейере инструкции.
;		Сам прыжок: 	jmp <сегмент>:<адрес смещения>
; ------------------------------------------------------------------------------

[bits 16]                ; Код работает в 16-битном реальном режиме

switch_to_pm:
	cli                   ; Отключаем прерывания, чтобы избежать проблем во время перехода

	lgdt [gdt_descriptor] ; Загружаем указатель на GDT (глобальную таблицу дескрипторов)
	                      ; В защищённом режиме сегментные регистры используют GDT

	mov eax, cr0          ; Загружаем текущее значение регистра CR0 (контрольный регистр)
	or eax, 0x1           ; Устанавливаем бит PE (Protection Enable) — включаем защищённый режим
	mov cr0, eax          ; Записываем обратно в CR0 — теперь процессор в защищённом режиме

	jmp CODE_SEG:init_pm  ; Делаем *far jump* (сегмент:смещение) для сброса очереди команд
	                      ; CODE_SEG — это селектор сегмента кода в GDT
	                      ; init_pm — адрес начала 32-битного кода

[bits 32]                ; С этого момента процессор работает в 32-битном режиме

init_pm:
	; Устанавливаем все сегментные регистры на сегмент данных
	mov ax, DATA_SEG      ; DATA_SEG — селектор сегмента данных из GDT
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; Настраиваем стек
	mov ebp, 0x90000      ; Устанавливаем указатель базы стека
	mov esp, ebp          ; И стековый указатель

	; Переход к основной функции ядра (32-битный режим)
	call BEGIN_PM         ; Вызов основной функции (может быть, kmain или что-то другое)
