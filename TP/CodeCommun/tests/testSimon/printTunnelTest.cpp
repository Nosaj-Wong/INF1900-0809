#include "functions.h"
#include "debug.h"

int main()
{
  DDRD = 0x00;

  const uint16_t N_TOTAL_ELEMENTS = 500;
  uint16_t leftDistances[N_TOTAL_ELEMENTS];
  uint16_t rightDistances[N_TOTAL_ELEMENTS];

  const uint16_t MAX_HALF_DISTANCE = 24000;
  uint16_t length = MAX_HALF_DISTANCE;

  for (;;)
  {
    if (PIND & 0x04)
    {
      _delay_ms(10);
      if (PIND & 0x04)
      {
        for (uint16_t i = 0; i < N_TOTAL_ELEMENTS; i++)
        {
          rightDistances[i] = length;
          leftDistances[i] = length;
          length -= MAX_HALF_DISTANCE / N_TOTAL_ELEMENTS; /// 93.75 = 93

          char resultat[10];
          intToString(length, resultat);
          DEBUG_PRINT((resultat));
        }

        printTunnel(N_TOTAL_ELEMENTS, leftDistances, rightDistances);
      }
    }
  }
}
