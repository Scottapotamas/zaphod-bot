#define PIN_RED 8
#define PIN_GREEN 9
#define PIN_BLUE 10
#define PIN_ENABLE 11
#define PIN_FAULT 12

#define PIN_DEBUG_LED 13

//PWM value which will be manipulated over time
//start them out of phase
uint8_t red_pwm   = 0;
uint8_t green_pwm = 86;
uint8_t blue_pwm  = 168;

uint8_t red_cycle   = 0;
uint8_t green_cycle = 0;
uint8_t blue_cycle  = 0;

enum led_cycle {
  INCREASING = 0,
  DECREASING,
};

void
pin_setup( void )
{
  pinMode( PIN_RED, OUTPUT );
  pinMode( PIN_GREEN, OUTPUT );
  pinMode( PIN_BLUE, OUTPUT );
  pinMode( PIN_ENABLE, OUTPUT );
  pinMode( PIN_FAULT, INPUT_PULLUP );
  pinMode( PIN_DEBUG_LED, OUTPUT );

  digitalWrite( PIN_RED, LOW );
  digitalWrite( PIN_GREEN, LOW );
  digitalWrite( PIN_BLUE, LOW );
  digitalWrite( PIN_ENABLE, LOW );
  digitalWrite( PIN_DEBUG_LED, LOW );

}

void
setup( void ) 
{  
  pin_setup();

  delay(500);

  enable_driver( true );
}

void
loop( void) 
{
  calculate_brightness( &red_pwm, &red_cycle );
  calculate_brightness( &blue_pwm, &blue_cycle );
  calculate_brightness( &green_pwm, &green_cycle );

  analogWrite( PIN_RED,   red_pwm );
  analogWrite( PIN_GREEN, green_pwm );
  analogWrite( PIN_BLUE,  blue_pwm );

  digitalWrite(PIN_DEBUG_LED, !digitalRead(PIN_DEBUG_LED));
  delay(10);

}

void
calculate_brightness( uint8_t *pwm_value, uint8_t *cycle_state )
{
  //generate a linear triangle
  if( *pwm_value >= 254 )
  {
    *cycle_state = 0;
  }
  else if( *pwm_value <= 0 )
  {
    *cycle_state = 1;
  }
  
  if( *cycle_state == 1 )
  {
    *pwm_value += 1;
  }
  else
  {
    *pwm_value -= 1;
  }
}

void
enable_driver( bool enable )
{
  digitalWrite( PIN_ENABLE, enable );
}
