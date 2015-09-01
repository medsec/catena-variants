#pragma once

#include "catena.hpp" 

namespace Catena_Variants{

/* Writes the XOR of the H_LEN long sequences input1 and input2  to output
* Uses Intrinsic to speed up computation when SSE2 is avaiable
*/
void XOR(const uint8_t *input1, const uint8_t *input2, uint8_t *output, uint16_t LEN);


/* Safely erase the pwd by making sure the compiler isn't skipping this step
 */
void erasepwd(uint8_t* pwd, const uint32_t pwdlen);

} //end namespace