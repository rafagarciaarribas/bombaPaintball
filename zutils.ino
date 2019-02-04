void drawPorcent(byte porcent){
  //TODO: Optimize this code 
  int aDibujar=(8*porcent)/10;
  lcd.setCursor(0,1);

  if(aDibujar<5)
  {
    switch(aDibujar){
    case 0:
      break;
    case 1:
      lcd.write((uint8_t)0);
      break;
    case 2:
      lcd.write(1);
      break;
    case 3:
      lcd.write(2);
      break;
    case 4:
      lcd.write(3);
      break;
    }
  }
  while(aDibujar>=5){
    if(aDibujar>=5)
    {
      lcd.write(4);
      aDibujar-=5;
    }
    if(aDibujar<5)
    {
      switch(aDibujar){
      case 0:
        break;
      case 1:
        lcd.write((uint8_t)0);
        break;
      case 2:
        lcd.write(1);
        break;
      case 3:
        lcd.write(2);
        break;
      case 4:
        lcd.write(3);
        break;
      }
    }
  }
}


void cls(){
  lcd.clear();
  lcd.setCursor(0,0);
}
