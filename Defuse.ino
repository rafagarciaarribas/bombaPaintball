void defuse(int ad){
  unsigned int percent=0;
unsigned long xTime=millis();
  while(defuseando)
    {
      cls();
      while(defuseando){
      
    /* // lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("PLANTANDO");
      lcd.setCursor(0,1);*/

      if (activada==HIGH){
        lcd.setCursor(2,0);
          lcd.print("DESACTIVANDO");
          lcd.setCursor(0,1);
     } 
      else{
        lcd.setCursor(4,0);
        lcd.print("PLANTANDO");
        lcd.setCursor(0,1);
      }
      
      unsigned long seconds= millis() - xTime;
      percent = (seconds)/(tiempoActivacion*10);
      drawPorcent(percent);
      defuseando=digitalRead(10);

      if(percent > 100)
        {
          cls();
          switch(ad){
            case 1:
            lcd.print("BOMBA ACTIVADA");
            pitidoContinuo();
            activada=HIGH; //le indicamos que la bomba ha sido activada para que nos muestre el texto: "Explosion en:"
            segundosTotal=210;
            cuentaAtrasActivada();
            break;
            
            case 2:
            lcd.print("BOMBA DESACTIVADA");
            pitidoContinuo();
            delay(100000);
            break;
            
           default:
                // statements
              break;
          } 
        }
      }
    }
}
