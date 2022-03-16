#ifndef _ENV_PICORV32_TEST_H
#define _ENV_PICORV32_TEST_H

#define RVTEST_RV32U
#define RVTEST_RV64U RVTEST_RV32U
#define TESTNUM x28
#define DEBUG_REG 0xfffffff0

#define RVTEST_CODE_BEGIN		\
	.text;				\
	.global test;			\
test:

#define RVTEST_PASS			\
.pass:					\
	li	a0,DEBUG_REG;		\
	li	a1,0x1;			\
	sw	a1,0(a0);		\
	j .pass;

#define RVTEST_FAIL			\
.fail:					\
	li	a0,DEBUG_REG;		\
	li	a1,0x0;			\
	sw	a1,0(a0);		\
	j .fail;

#define RVTEST_CODE_END
#define RVTEST_DATA_BEGIN .balign 4;
#define RVTEST_DATA_END

#endif
