APR = 0
PI = 4
PI_RESET = 10000
PI_SET_PIR = 4000
PI_SET_PIO = 2000
PI_CLR_PIO = 1000
PI_CLR_ACT = 400
PI_SET_ACT = 200

. = 40
	777777777777
	JRST	4,

. = 1000
#	CONI	PI,1
#	CONO	PI,PI_RESET
#	CONO	PI,PI_SET_ACT|PI_SET_PIO|177
#	DATAO	APR,APR_PR_RLR
#	DATAI	APR,2
#	CONI	APR,1
#	BLKI	APR,TEST
#	JRST	4,
#	JRST	.-2
#	MOVE	0,BLTTEST
#	BLT	0,BLTSPC+17
#	XCT	XCTTEST
	UUO04	3,BLAH
	JRST	4,

TEST:
	-5,,TEST+1
	. = . + 10
XCTTEST:
	MOVEI	0,4321
BLTSPC:
	. = . + 20
APR_PR_RLR:
	321321123123
BLTTEST:
	1,,BLTSPC
BLAH:
	1234,,4321
