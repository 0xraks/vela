void forward()
{digitalWrite(5,HIGH);
digitalWrite(18,LOW);
digitalWrite(19,HIGH);
digitalWrite(21,LOW);
}

void backward()
{digitalWrite(5,LOW);
digitalWrite(18,HIGH);
digitalWrite(19,LOW);
digitalWrite(21,HIGH);
}
void left()
{digitalWrite(5,LOW);
digitalWrite(18,HIGH);
digitalWrite(19,HIGH);
digitalWrite(21,LOW);
}
void right()
{digitalWrite(5,HIGH);
digitalWrite(18,LOW);
digitalWrite(19,LOW);
digitalWrite(21,HIGH);
}
void stopbot()
{digitalWrite(5,LOW);
digitalWrite(18,LOW);
digitalWrite(19,LOW);
digitalWrite(21,LOW);
}
