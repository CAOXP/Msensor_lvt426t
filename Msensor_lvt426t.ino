


bool sendXYanglebyRF()
{
	uint8_t i=0;
#if RF_TRANSPANRENT == 0
	Ser2Rf.write(0xD0);		//data head
	Ser2Rf.write(0x06);		//data length
	Ser2Rf.write(RF_RECEIVE_ADDRSS_HIGH_BYTE);		//address High
	Ser2Rf.write(RF_RECEIVE_ADDRSS_LOW_BYTE);		//address LOW 
#endif
	for(i=0;i<6;i++)
	{
		Ser2Rf.write(dataXYstr[i]);
	}

	Ser2Rf.listen();
	//3.waiting for data back
	while ( !(Ser2Rf.available() > 0 ) )
	{;}
	//4.read ok
	i=0;
	unsigned char rcvStrtmp[10];
	while ( Ser2Rf.available() > 0 )
	{
		rcvStrtmp[i] = Ser2Rf.read();
		i++;
		delay(10);
		if (i > 6)
		{
		  i--;	//全部接收完
		}
	}

	if(    (    (rcvStrtmp[0]=='O') || (rcvStrtmp[0]=='o')   )
		&& (    (rcvStrtmp[1]=='K') || (rcvStrtmp[1]=='k')   )
		)
	{
		return true;
	}


	return false;
}