void readExpander(){
    Wire.beginTransmission(0x20);
     Wire.write(0x12); // set MCP23017 memory pointer to GPIOB address
     Wire.endTransmission();
     Wire.requestFrom(0x20, 1); // request one byte of data from MCP20317
     byte data=Wire.read(); //receive data in binary
}
