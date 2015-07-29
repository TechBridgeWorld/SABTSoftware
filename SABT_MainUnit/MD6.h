/**
 * @file MD6.h
 * @brief Declarations for mode 6 - Free Play
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _MD6_H_
#define _MD6_H_


// State definitions
#define MD6_STATE_INITIAL   0b00000000  // Initial state
#define MD6_STATE_INPUT     0b00000001  // Waits for dot input from user
#define MD6_STATE_CHECK     0b00000010  // Verifies input, gives feedback, returns to input wait loop


void md6_main(void);
void md6_reset(void);

#endif /* _MD6_H_ */
