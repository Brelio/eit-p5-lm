/******************************************************
*                                                    *
*  __  _    ___  ____   ____     ___  _              *
* |  |/ ]  /  _]|    \ |    \   /  _]| |             *
* |  ' /  /  [_ |  D  )|  _  | /  [_ | |             *
* |    \ |    _]|    / |  |  ||    _]| |___          *
* |     ||   [_ |    \ |  |  ||   [_ |     |         *
* |  .  ||     ||  .  \|  |  ||     ||     |         *
* |__|\_||_____||__|\_||__|__||_____||_____|         *
*                                                    *
*                                                    *
* krnl.h                                             *
*                                                    *
*      March 2015                                    *
*      Author: jdn                                   *
**                                                   *
******************************************************
*            (simple not - not ?! :-) )              *
* my own small KeRNeL adapted for Arduino            *
*                                                    *
* previous known as KRNL                             *
*                                                    *
* this version adapted for Arduino                   *
*                                                    *
* (C) 2012,2013,2014                                 *
*                                                    *
* Jens Dalsgaard Nielsen <jdn@es.aau.dk>             *
* http://es.aau.dk/staff/jdn                         *
* Section of Automation & Control                    *
* Aalborg University,                                *
* Denmark                                            *
*                                                    *
* "THE BEER-WARE LICENSE" (frit efter PHK)           *
* <jdn@es.aau.dk> wrote this file. As long as you    *
* retain this notice you can do whatever you want    *
* with this stuff. If we meet some day, and you think*
* this stuff is worth it ...                         *
*  you can buy me a beer in return :-)               *
* or if you are real happy then ...                  *
* single malt will be well received :-)              *
*                                                    *
* Use it at your own risk - no warranty              *
*                                                    *
* tested with duemilanove w/328, uno R3,             *
* seeduino 1280 and mega2560                         *
*****************************************************/
// remember to update in krnl.c !!!
#define KRNL_VRS 2002

/***********************
NB NB ABOUT TIMERS PORTS ETC

You can configure krnl to use timer 0,1,2,3,4,5
Buth normally you cant use timer 0 bq it is used for millis and preallocated.

See below

When using a timer you must be aware of that it will prohibit you from things like
- tone (pwm sound) uses timer2

... from http://blog.oscarliang.net/arduino-timer-and-interrupt-tutorial/

Timer0:
- Timer0 is a 8bit timer.
- In the Arduino world Timer0 is been used for the timer functions, like delay(), millis() and micros().
-  If you change Timer0 registers, this may influence the Arduino timer function.
- So you should know what you are doing.

- Timer1 is a 16bit timer.
- In the Arduino world the Servo library uses Timer1 on Arduino Uno (Timer5 on Arduino Mega).

Timer2:
- Timer2 is a 8bit timer like Timer0.
 -In the Arduino work the tone() function uses Timer2.

Timer3, Timer4, Timer5: Timer 3,4,5 are only available on Arduino Mega boards.
fe- These timers are all 16bit timers.


On Uno
- Pins 5 and 6: controlled by timer0
- Pins 9 and 10: controlled by timer1
- Pins 11 and 3: controlled by timer2

On the Arduino Mega we have 6 timers and 15 PWM outputs:

- Pins 4 and 13: controlled by timer0
- Pins 11 and 12: controlled by timer1
- Pins 9 and10: controlled by timer2
- Pin 2, 3 and 5: controlled by timer 3
- Pin 6, 7 and 8: controlled by timer 4
- Pin 46, 45 and 44:: controlled by timer 5

... from http://arduino-info.wikispaces.com/Timers-Arduino

- Servo Library uses Timer1.
-- You can’t use PWM on Pin 9, 10 when you use the Servo Library on an Arduino.
-- For Arduino Mega it is a bit more difficult. The timer needed depends on the number of servos.
-- Each timer can handle 12 servos.
-- For the first 12 servos timer 5 will be used (losing PWM on Pin 44,45,46).
-- For 24 Servos timer 5 and 1 will be used (losing PWM on Pin 11,12,44,45,46)..
-- For 36 servos timer 5, 1 and 3 will be used (losing PWM on Pin 2,3,5,11,12,44,45,46)..
-- For 48 servos all 16bit timers 5,1,3 and 4 will be used (losing all PWM pins).

- Pin 11 has shared functionality PWM and MOSI.
-- MOSI is needed for the SPI interface, You can’t use PWM on Pin 11 and the SPI interface at the same time on Arduino.
-- On the Arduino Mega the SPI pins are on different pins.

- tone() function uses at least timer2.
-- You can’t use PWM on Pin 3,11 when you use the tone() function an Arduino and Pin 9,10 on Arduino Mega.

SO BEWARE !!!

***********************/

#ifndef KRNL

#define KRNL

// if krnl shall support EDF scheduling
#define EDF
// DISPLAY OF PROCESS ID BY LEDS if you want to
#define KRNLBUG

// USER CONFIGURATION PART

/* which timer to use for krnl heartbeat
* timer 0 ( 8 bit) is normally used by millis - avoid !
* timer 1 (16 bit)  DEFAULT
* timer 2 ( 8 bit)
* timer 3 (16 bit) 1280/2560 only (MEGA)
* timer 4 (16 bit) 1280/2560 only (MEGA)
* timer 5 (16 bit) 1280/2560 only (MEGA)
*/

#if defined (__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define KRNLTMR 1
#else
#define KRNLTMR 1
#endif
// END USER CONFIGURATION




//----------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

// if you are using k_mutex with prio inheritance
// #define MUTEX
// for guessing on architecture ...

#if defined(__AVR_ATmega168__)
#define ARCH_SLCT 1
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__)
#define ARCH_SLCT 2
#elif defined(__AVR_ATmega32U4__)
#define ARCH_SLCT 3
#elif defined (__AVR_ATmega1280__)
#define ARCH_SLCT 4
#elif defined(__AVR_ATmega2560__)
#define ARCH_SLCT 6
#else
#error Failing due to unknown architecture - krnl
#endif


#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega32U4__)

#if (KRNLTMR != 0) && (KRNLTMR != 1) &&(KRNLTMR != 2)
#error "bad timer selection for krnl heartbeat(168/328/328p/...)"
#endif

#endif

#if defined (__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#if (KRNLTMR != 0) && (KRNLTMR != 1) && (KRNLTMR != 2) && (KRNLTMR != 3) && (KRNLTMR != 4) && (KRNLTMR != 5)
#error "bad timer for krnl heartbeat(1280/2560) in krnl"
#endif

#endif


// DEBUGGING
//#define DMYBLINK              // ifdef then led (pin13) will light when dummy is running

#define QHD_PRIO 100		    // Queue head prio - for sentinel use
#define DMY_PRIO (QHD_PRIO-2)	// dummy task prio (0 == highest prio)
#define DMY_STK_SZ  90		    // staksize for dummy
#define MAIN_PRIO   50		    // main task prio
#define STAK_HASH   0x5c	    // just a hashcode
#define MAX_SEM_VAL 50		    // NB is also max for nr elem in msgQ !
#define MAX_INT 0x7FFF
#define SEM_MAX_DEFAULT 50

#ifdef EDF
#define EDF_PRIO_LIMIT 29999
#endif

extern int k_task, k_sem, k_msg;
extern volatile char krnl_preempt_flag;
extern char dmy_stk[DMY_STK_SZ];

/***** KeRNeL data types *****/
struct k_t {
    unsigned char nr;
    struct k_t *next,	        // task,sem: double chain lists ptr
            *pred;			    // task,sem: double chain lists ptr
    volatile char sp_lo,	    // sem:vacant    | task: low 8 byte of stak adr
             sp_hi;		        // sem: vacant   |task: high 8 byte of stak adr
                                // edf needs large prio for having long deadlines
#ifdef EDF
    int
#else
    char
#endif
    prio;			            // task & sem:  priority
    volatile int cnt1,	        // sem: sem counter | task: ptr to stak
             cnt2,			    // asem: dyn part of time counter | task: timeout
             cnt3,			    // sem: preset timer value |  task: ptr to Q we are hanging in
             maxv,			    // sem: max value |         task: org priority
             clip;			    // sem: counter for lost signals | task: vacant
};

struct k_msg_t {
    // msg type
    unsigned char nr;
    struct k_t *sem;
    char *pBuf;		            // ptr to user supplied ringbuffer
    volatile int nr_el, el_size, lost_msg;
    volatile int r, w, cnt;
};

/***** KeRNeL variables *****/
extern struct k_t *task_pool,
        *sem_pool, AQ,	        // activeQ
        *pmain_el, *pAQ, *pDmy,	// ptr to dummy task descriptor
        *pRun,			        // ptr to running task
        *pSleepSem;

extern struct k_msg_t *send_pool;

extern char nr_task, nr_sem, nr_send;

extern volatile char k_running;	// no running

extern volatile char k_err_cnt;	// every time an error occurs cnt is incr by one


/******************************************************
 * MACROS MACROS
 *
 * PUSHREGS AND POPREGS
 * is actual staklayout plus task address at top
 * A push/pop takes 2 cycles
 * a call takes 3 cycles
 * ret/iret 3-4 cycles
 * So a PUSHREGS is 33 instructions(@ 2 cycles) = 66  cycles ~= 66 cycles /"16 MHz" ~= 4.1 usec
 * So an empty iSR which preserves all registers takes 2*4.1usec + 8-10 cycles (intr + iret) ~= 9 usec
 * So max isr speed with all regs saved is around 100 kHz but then with no code inside ISR !
 *
 * WARNING
 * The 2560 series has 3 bytes PC the rest only 2 bytes PC !!! (PC. program counter)
 * and no tall has rampz and eind register
 *
 *
 * REGISTER NAMING AND INTERNAL ADRESSING
 *
 * Register addresses
 * 0x3f SREG
 * 0x3e SPH
 * 0x3d SPL
 * 0x3c EIND
 * 0x3b RAMPZ
 * ...
 * 0x1f R31
 * etc
 * 0x01 R1
 * 0x00 R0
 *
 * PC is NOT available
 */


#define lo8(X) ((unsigned char)((unsigned int)(X)))
#define hi8(X) ((unsigned char)((unsigned int)(X) >> 8))

extern volatile char k_bug_on;

#ifdef KRNLBUG

#define K_CHG_STAK()    \
if (pRun != AQ.next) {  \
  pRun->sp_lo = SPL;    \
  pRun->sp_hi = SPH;    \
  pRun = AQ.next;       \
  k_breakout();         \
  SPL = pRun->sp_lo;    \
  SPH = pRun->sp_hi;    \
}

#else

#define K_CHG_STAK()    \
if (pRun != AQ.next) {  \
  pRun->sp_lo = SPL;    \
  pRun->sp_hi = SPH;    \
  pRun = AQ.next;       \
  SPL = pRun->sp_lo;    \
  SPH = pRun->sp_hi;    \
}

#endif

// MISSING no code 1284p

#define DI()   asm volatile ("cli")
#define EI()   asm volatile ("sei")
#define RETI() asm volatile ("reti")

/* below: r1 must/shall always assumed to be zero in c code (gcc issue I think) */

#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega1280__)

// 0x3b RAMPZ extended z-pointer register
// 0x3c EIND extended indirect register

#define PUSHREGS() asm volatile ( \
"push r1  \n\t" \
"push r0  \n\t" \
"in r0, __SREG__ \n\t" \
"cli \n\t" \
"push r0  \n\t" \
"in r0 , 0x3b \n\t" \
"push r0 \n\t" \
"in r0 , 0x3c \n\t" \
"push r0 \n\t" \
"clr r1 \n\t" \
"push r2  \n\t" \
"push r3  \n\t" \
"push r4  \n\t" \
"push r5  \n\t" \
"push r6  \n\t" \
"push r7  \n\t" \
"push r8  \n\t" \
"push r9  \n\t" \
"push r10 \n\t" \
"push r11 \n\t" \
"push r12 \n\t" \
"push r13 \n\t" \
"push r14 \n\t" \
"push r15 \n\t" \
"push r16 \n\t" \
"push r17 \n\t" \
"push r18 \n\t" \
"push r19 \n\t" \
"push r20 \n\t" \
"push r21 \n\t" \
"push r22 \n\t" \
"push r23 \n\t" \
"push r24 \n\t" \
"push r25 \n\t" \
"push r26 \n\t" \
"push r27 \n\t" \
"push r28 \n\t" \
"push r29 \n\t" \
"push r30 \n\t" \
"push r31 \n\t" \
)

#define POPREGS() asm volatile ( \
"pop r31 \n\t" \
"pop r30 \n\t" \
"pop r29 \n\t" \
"pop r28 \n\t" \
"pop r27 \n\t" \
"pop r26 \n\t" \
"pop r25 \n\t" \
"pop r24 \n\t" \
"pop r23 \n\t" \
"pop r22 \n\t" \
"pop r21 \n\t" \
"pop r20 \n\t" \
"pop r19 \n\t" \
"pop r18 \n\t" \
"pop r17 \n\t" \
"pop r16 \n\t" \
"pop r15 \n\t" \
"pop r14 \n\t" \
"pop r13 \n\t" \
"pop r12 \n\t" \
"pop r11 \n\t" \
"pop r10 \n\t" \
"pop r9  \n\t" \
"pop r8  \n\t" \
"pop r7  \n\t" \
"pop r6  \n\t" \
"pop r5  \n\t" \
"pop r4  \n\t" \
"pop r3  \n\t" \
"pop r2  \n\t" \
"pop r0  \n\t" \
"out 0x3c , r0 \n\t " \
"pop r0  \n\t" \
"out 0x3b , r0 \n\t " \
"pop r0  \n\t" \
"out __SREG__ , r0 \n\t " \
"pop r0  \n\t" \
"pop r1  \n\t" \
)

#else

#define PUSHREGS() asm volatile ( \
"push r1 \n\t" \
"push r0 \n\t" \
"in r0, __SREG__ \n\t" \
"cli \n\t" \
"push r0  \n\t" \
"clr r1 \n\t" \
"push r2  \n\t" \
"push r3  \n\t" \
"push r4  \n\t" \
"push r5  \n\t" \
"push r6  \n\t" \
"push r7  \n\t" \
"push r8  \n\t" \
"push r9  \n\t" \
"push r10 \n\t" \
"push r11 \n\t" \
"push r12 \n\t" \
"push r13 \n\t" \
"push r14 \n\t" \
"push r15 \n\t" \
"push r16 \n\t" \
"push r17 \n\t" \
"push r18 \n\t" \
"push r19 \n\t" \
"push r20 \n\t" \
"push r21 \n\t" \
"push r22 \n\t" \
"push r23 \n\t" \
"push r24 \n\t" \
"push r25 \n\t" \
"push r26 \n\t" \
"push r27 \n\t" \
"push r28 \n\t" \
"push r29 \n\t" \
"push r30 \n\t" \
"push r31 \n\t" \
)

#define POPREGS() asm volatile ( \
"pop r31 \n\t" \
"pop r30 \n\t" \
"pop r29 \n\t" \
"pop r28 \n\t" \
"pop r27 \n\t" \
"pop r26 \n\t" \
"pop r25 \n\t" \
"pop r24 \n\t" \
"pop r23 \n\t" \
"pop r22 \n\t" \
"pop r21 \n\t" \
"pop r20 \n\t" \
"pop r19 \n\t" \
"pop r18 \n\t" \
"pop r17 \n\t" \
"pop r16 \n\t" \
"pop r15 \n\t" \
"pop r14 \n\t" \
"pop r13 \n\t" \
"pop r12 \n\t" \
"pop r11 \n\t" \
"pop r10 \n\t" \
"pop r9  \n\t" \
"pop r8  \n\t" \
"pop r7  \n\t" \
"pop r6  \n\t" \
"pop r5  \n\t" \
"pop r4  \n\t" \
"pop r3  \n\t" \
"pop r2  \n\t" \
"pop r0  \n\t" \
"out __SREG__ , r0 \n\t " \
"pop r0  \n\t" \
"pop r1  \n\t" \
)

#endif				// 1280/2560

// function prototypes
// naming convention
// k_... function do a DI/EI and can impose task shift
// ki_... expects interrupt to be disablet and do no task shift
// rest is internal functions


/**
* millis in krnle - NB steps equals milli seconds given in k_start
*/
unsigned long k_millis (void);


/**
 * Eats CPU time in 1 msec quants
 * @param[in] eatTime  number of milliseconds to eay (<= 10000
 */
void k_eat_time (unsigned int eatTime);

/**
* issues a task shift - handle with care
* Not to be used by normal user
*/
void ki_task_shift (void) __attribute__ ((naked));

/**
* Set task asleep for a number of ticks.
* @param[in] time nr of ticks to sleep < 0
* @remark only to be called after start of KRNL
*/
int k_sleep (int time);

/**
* creates a task and put it in the active Q
* @param[in] pTask pointer to function for code ( void task(void) ...)
* @param[in] prio Priority 1: highest (QHEAD_PRIO-1): lowest
* @param[in] pStk Reference to data area to be used as stak
* @param[in] stkSize size of data area(in bytes) to be used for stak
* @return: pointer to task handle or NULL if no success
* @remark only to be called before start of KRNL but after k_init
*/
struct k_t *k_crt_task (void (*pTask) (void), char prio, char *pStk,
                        int stkSize);

/**
* change priority of calling task)
* @param[in] prio new prio, Priority 1: highest (QHEAD_PRIO-1): lowest
* @return: 0: ok, -1: KRNL not running, -2: illegal value
* @remark only to be called after start of KRNL
*/
int k_set_prio (char prio);


/**
* creates a standard Dijkstra semaphore. It can be initialized to values in range [0..maxvalue]
* @param[in] init_val startvalue of semaphore 0,1,2,... maxvalue
* @param[in] maxvalue which is maxvalue semaphore can reach
* @return handle to semaphore or NULL pointer
* @remark only to be called before start of KRNL
*/
struct k_t *k_crt_sem (char init_val, int maxvalue);

/**
* attach a timer to the semaphore so KRNL will signal the semaphore with regular intervals.
* Can be used for cyclic real time run of a task.
* @param[in] sem semaphore handle
* @param[in] val interval in quant of KRNL ticks (0: disable cyclic timer, 1,2,3... cyclic quant)
* @return -1: negative val, 0. ok
* @remark only to be called after start of KRNL
*/
int k_set_sem_timer (struct k_t *sem, int val);

/**
* Signal a semaphore. Can be called from an ISR when interrupt is disabled. No task shift will occur - only queue manipulation.
* @param[in] sem semaphore handle
* @return 0: ok , -1: max value of semaphore reached
* @remark only to be called after start of KRNL
*/
int ki_signal (struct k_t *sem);

/**
* Signal a semaphore. Task shift will task place if a task is started by the signal and has higher priority than you.
* @param[in] sem semaphore handle
* @return 0: ok , -1: max value of semaphore reached
* @remark The ki_ indicates that interrups is NOT enabled when leaving ki_signal
* @remark only to be called after start of KRNL
*/
int k_signal (struct k_t *sem);

/**
* Signal a semaphore. Task shift will task place if a task is started by the signal and has higher priority than you.
* you shall supply with priority for prio ceiling protocol
* @param[in] sem semaphore handle
* @param[in] prio NOT WORKING DONT USE CALL
* @return 0: ok , -1: max value of semaphore reached
* @remark The ki_ indicates that interrups is NOT enabled when leaving ki_signal
* @remark only to be called after start of KRNL
*/
int k_prio_signal (struct k_t *sem, char prio);


/**
* Wait on a semaphore. Task shift will task place if you are blocked.
* @param[in] sem semaphore handle
* @param[in] timeout "<0" you will be started after timeout ticks, "=0" wait forever "-1" you will not wait
* @return 1: ok there was a signal hanging - so no suspension
* @return 0: ok- you have been suspended
* @return -1: timeout has occured, -2 no wait bq timeout was -1 and semaphore was negative
* @remark only to be called after start of KRNL
*/
int k_wait (struct k_t *sem, int timeout);


/**
* Wait on a semaphore. Task shift will task place if you are blocked.
* you shall supply with priority for prio ceiling protocol
* @param[in] sem semaphore handle
* @param[in] prio NOT WORKING DONT USE CALL
* @param[in] timeout "<0" you will be started after timeout ticks, "=0" wait forever "-1" you will not wait
* @return 1, ok : no suspension, 0: ok you ahv ebeen sleeping, -1: timeout has occured, -2 no wait bq timeout was -1 and semaphore was negative
* @remark only to be called after start of KRNL
*/
int k_prio_wait (struct k_t *sem, int timeout, char prio);

/**
* Wait on a semaphore. Task shift will task place if you are blocked.
* @param[in] sem semaphore handle
* @param[in] timeout "<0" you will be started after timeout ticks, "=0" wait forever "-1" you will not wait
* @param[out] lost if  not eq NULL it resturns how many signals has been lost
* @return 0: ok , -1: timeout has occured, -2 no wait bq timeout was -1 and semaphore was negative
* @remark only to be called after start of KRNL
*/
int k_wait_lost (struct k_t *sem, int timeout, int *lost);


/**
* Returns how many signals has been lost on semaphore due to saturation
* @param[in] sem semaphore handle
* @return nr of signals lost
* @remark only to be called after start of KRNL
*/
int k_sem_signals_lost (struct k_t *sem);

/**
* Like k_wait with the exception interrupt is NOT enabled when leaving
* @param[in] sem semaphore handle
* @param[in] timeout "<0" you will be started after timeout ticks, "=0" wait forever "-1" you will not wait
* @return 0: ok , -1: could do wait bw blocking would have taken place
* @remark The ki_ indicates that interrups is NOT enabled when leaving ki_wait
* @remark only to be called after start of KRNL
*/
int ki_wait (struct k_t *sem, int timeout);

/**
* returns value of semaphore
* @param[in] sem semaphore handle
* @return 1: ok not suspended, 0: ok you have been suspended
* @return -1 no wait maybe bq no timeout was allowed
* @remark only to be called after start of KRNL
*/
int ki_semval (struct k_t *sem);

/**
* a function for overloading on usersite which is called when a semaphore is overflooding
* no reset occur - it's only reading out semaphore idendity
* Signal operations has not taken place !
* 1: means first semahore allocated by user etc
* Interrupt is disabled when called and must not be enabled during.. so no print etc
* @param[in] nr  id of semaphore 1,2,3,...
* @param[in] nrClip number of times clip has occured (may be reset by call k_wait_lost)
*/
#ifdef KRNLBUG
void __attribute__ ((weak)) k_sem_clip (unsigned char nr, int nrClip);
#endif

/**
* a function for overloading on usersite which is called when a msgQ is overflooding
* no reset occur - it's only readind out smsgQ idendity
* 1: means first msgQ allocated by user etc
* Interrupt is disabled when called and must not be enabled during.. so no print etc
* @param nr : id of send Q 0,1,2,...
* @param nrClip: number of times clip has occured (may be reset by call k_receive and lost parm not eq NULL)
*/
#ifdef KRNLBUG
void __attribute__ ((weak)) k_send_Q_clip (unsigned char nr, int nrClip);
#endif

struct k_msg_t *k_crt_send_Q (int nr_el, int el_size, void *pBuf);

/**
* Put data (one element of el_size)in the ringbuffer if there are room for it.
* Intended for ISR use
* DONE BY COPY !
* @param[in] pB Ref to message buffer
* @param[in] el Reference to data to be put in buffer. Size if already given in k_crt_send
* @return 0: operation did succed, -1: no room in ringbuffer
* @remark Interrupt will not enabled upon leaving, so ki_send is intended to be used from an ISR
* @remark only to be called before start of KRNL
*/
char ki_send (struct k_msg_t *pB, void *el);


/**
* Put data (one element of el_size)in the ringbuffer if there are room for it.
* DONE BY COPY !
* @param[in] pB Ref to message buffer
* @param [in] el Reference to data to be put in buffer. Size if already given in k_crt_send
* @return 0: operation did succed, -1: no room in ringbuffer
* @remark only to be called after start of KRNL
* @remark k_send does not block if no space in buffer. Instead -1 is returned
*/
char k_send (struct k_msg_t *pB, void *el);

/**
* Receive data (one element of el_size)in the ringbuffer if there are data
* DONE BY COPY !
* @param[in] pB Ref to message buffer
* @param [out] el Reference to where data shall be copied to at your site
* @param[in] timeout Max time you want to wait on data, -1: never, 0: forever, positive: nr of KRNL timer quants
* @param[out] lost_msg nr of lost messages since last receive. will clip at 10000. If lost_msg ptr is NULL then overrun counter
* is not reset to 0.
* @return 1: ok no suspension, 0: operation did succed, -1: no data in ringbuffer
* @remark only to be called after start of KRNL
*/
char k_receive (struct k_msg_t *pB, void *el, int timeout, int *lost_msg);

/**
* Receive data (one element of el_size)in the ringbuffer if there are data
* DONE BY COPY !
* No blocking if no data
* Interrupt will not be enabled after ki_receive
* @param[in] pB Ref to message buffer
* @param[out] el Reference to where data shall be copied to at your site
* @param[out] lost_msg nr of lost messages since last receive. will clip at 10000.  If lost_msg ptr is NULL then overrun counter
* is not reset to 0
* @return 1: data was rdy no suspension, 0: ok you have been suspended , -1: no data in ringbuffer
* @remark can be used from ISR
* @remark only to be called after start of KRNL
*/
char ki_receive (struct k_msg_t *pB, void *el, int *lost_msg);


/**
* returns which timer is used
* @return 0,1,2,3,4,5 ...
*/
int k_tmrInfo (void);	// tm in milliseconds


/**
* Initialise KRNL. First function to be called.
* You have to give max number of tasks, semaphores and message queues you will use
* @param[in] nrTask ...
* @param[in] nrSem ...
* @param[in] nrMsg ...
 */
int k_init (int nrTask, int nrSem, int nrMsg);

/**
* start KRNL with tm tick speed (1= 1 msec, 5 = 5 msec)
* @param[in] tm Tick length in milli seconds(1..10,20,30..10000
* @return  -1-333 nr of k_Crt calls taht went wrong. krnl did not start
*          -555 negative tick parm value
*          -666 bad tick quant (legal is 1-10,20,30-10000
* @remark only to be called after init of KRNL
* @remark KRNL WILL NOT START IF YOU HAVE TRIED TO CREATE MORE TASKS/SEMS/MSG QS THAN YOU HAVE ALLOCATED SPACE FOR IN k_init !!!
*/
int k_start (int tm);	// tm in milliseconds

/**
* stop KRNL
* @param[in] exitVal  Will be returned in k_start to creator (old main)
* @remark only to be called after k_start
* @remark you will only return from k_stop if krnl is not running
*/
int k_stop (int exitVal);	// tm in milliseconds


/**
* Reset by disable interrupt plus activate watchdog (15 millisseconds) and just wait...
**/
void k_reset ();

/**
* Initialise blink on pin 13
* ON when dummy is running
* Nov 2014 - fake do not use it bq it will not work
* for emergency use :-)
*/
void k_bugblink13 (char on);

/**
* returns nr of unbytes bytes on stak.
* For chekking if stak is too big or to small...
* @param[in] t Reference to task (by task handle) If null then yourself
* @return: nr of unused bytes on stak (int)
* @remark only to be called after start of KRNL
* @remark no chk of if it is a valid task
*/
int k_stk_chk (struct k_t *t);

/**
* Returns amount of unused stak
* @param[in] t  Ptr to taskdescriptor. If NULL it is yourself
* @return: amount of unused stak(in bytes)
* @remark: a watermark philosophy is used
**/
int k_unused_stak (struct k_t *t);


/**
* Set preempt or non preempt
* @param[in] on : 1: preempt on, 0: off 2: no change
* @return: current state: 1 preempt sch. 0 non preempt
**/
char k_set_preempt (char on);

/**
* Get preempt or non preempt
* @return: current state: 1 preempt sch. 0 non preempt
**/
char k_get_preempt (void);

/**
* returns amount of free memory in your system
*/
int freeRam (void);

#ifdef KRNLBUG

/**
* Breakout function called from scheduler
**/
void __attribute__ ((weak)) k_breakout (void);
#endif

#ifdef __cplusplus
}
#endif

#endif				// #ifndef KRNL
