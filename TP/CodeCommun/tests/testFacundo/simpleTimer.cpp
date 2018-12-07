// Simple timer functions

void setTimerB() {
  // OC3A mis sur "compare match", output = high level (p.128)
  TCCR1A |= (1 << COM1B1) | (1 << COM1B0);
  // Diminution de l'horloge a 1/1024 (p.131) (bit CS10 mit a 0)
  TCCR1B |= (1<<WGM12 | 1 << CS10) | (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1C = 0;
}

// start timer
void startTimerB(uint16_t duration) {
  TCNT1 = 0;
  OCR1A = 78 * duration;
  OCR1B = 78 * duration;
  TIMSK1 |= (1 << OCIE1B);
}

void stopTimerB(uint16_t duration) {
  TIMSK1 &= ~(1 << OCIE1B);
}