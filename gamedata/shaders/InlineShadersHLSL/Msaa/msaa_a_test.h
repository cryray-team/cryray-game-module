//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	MSAA_A_TEST_H
#define	MSAA_A_TEST_H
	#ifdef MSAA_ALPHATEST_LOW
	#if MSAA_SAMPLES == 2
		static const float2 MSAAOffsets[2] = 
		{ 
			float2(4.f,4.f), 
			float2(-4.f,-4.f) 
		};
	#endif
	#if MSAA_SAMPLES == 4
		static const float2 MSAAOffsets[4] = 
		{ 
			float2(-2.f,-6.f), 
			float2(6.f,-2.f),
			float2(-6.f,2.f), 
			float2(2.f,6.f) 
		};
	#endif
	#if MSAA_SAMPLES == 8
		static const float2 MSAAOffsets[8] = 
		{ 
			float2(1.f,-3.f), 
			float2(-1.f,3.f), 
			float2(5.f,1.f), 
			float2(-3.f,-5.f), 
			float2(-5.f,5.f), 
			float2(-7.f,-1.f), 
			float2(3.f,7.f), 
			float2(7.f,-7.f) 
		};
	#endif
	#endif
#if (defined(MSAA_ALPHATEST_HIGH) || defined(MSAA_ALPHATEST_LOW))
	#if MSAA_SAMPLES == 2
	uint alpha_to_coverage(float alpha, float2 pos2d)
	{
		uint mask;
		uint pos = uint(pos2d.x) | uint( pos2d.y);
		
		if (alpha < 0.3333f)
			mask = 0;
		else if(alpha < 0.6666f)
			mask = 1 << (pos & 1);
		else 
			mask = 3;

		return mask;
	}
	#endif
	#if MSAA_SAMPLES == 4
	uint alpha_to_coverage(float alpha, float2 pos2d)
	{
		uint mask;

		float off = float((uint(pos2d.x) | uint( pos2d.y)) & 3);
		alpha = saturate(alpha - off * ((0.2f / 4.f ) / 3.f));
		if (alpha < 0.40f)
		{
			if (alpha < 0.20f)
				mask = 0;	
			else if(alpha < 0.40f) // only one bit set
				mask = 1;
		}
	  else
	  {
			if(alpha < 0.60f) // 2 bits set => 1100 0110 0011 1001 1010 0101
			{
				mask = 3;
			}
			else if (alpha < 0.8f) // 3 bits set => 1110 0111 1011 1101 
				mask = 7;
			else
				mask = 0xf;
		}

		return mask;
	}
	#endif
	#if MSAA_SAMPLES == 8
	uint alpha_to_coverage(float alpha, float2 pos2d)
	{
		uint mask;

		float off = float((uint(pos2d.x) | uint(pos2d.y)) & 3);
		alpha = saturate(alpha - off * ((0.1111f / 8.0f) / 3.0f));

		if (alpha < 0.4444f)
		{
			if (alpha < 0.2222f)
			{
				if (alpha < 0.1111f)
					mask = 0;
				else // only one bit set 0.2222
					mask = 1;
			}
			else
			{
				if (alpha < 0.3333f) // 2 bits set0=> 10000001 + 11000000 .. 00000011 : 8 // 0.2222
								   //        set1=> 10100000 .. 00000101 + 10000010 + 01000001 : 8
								   //		set2=> 10010000 .. 00001001 + 10000100 + 01000010 + 00100001 : 8
								   //		set3=> 10001000 .. 00010001 + 10001000 + 01000100 + 00100010 + 00010001 : 8
				{
					mask = 3;
				}
				else // 3 bits set0 => 11100000 .. 00000111 + 10000011 + 11000001 : 8 ? 0.4444 // 0.3333
					 //        set1 => 10110000 .. 00001011 + 10000101 + 11000010 + 01100001: 8
					 //        set2 => 11010000 .. 00001101 + 10000110 + 01000011 + 10100001: 8
					 //        set3 => 10011000 .. 00010011 + 10001001 + 11000100 + 01100010 + 00110001 : 8
					 //        set4 => 11001000 .. 00011001 + 10001100 + 01000110 + 00100011 + 10010001 : 8
				{
					mask = 0x7;
				}
			}
		}
		else
		{
			if (alpha < 0.6666f)
			{
				if (alpha < 0.5555f) // 4 bits set0 => 11110000 .. 00001111 + 10000111 + 11000011 + 11100001 : 8 // 0.5555
								   //        set1 => 11011000 .. 00011011 + 10001101 + 11000110 + 01100011 + 10110001 : 8
								   //        set2 => 11001100 .. 00110011 + 10011001 : 4 make 8
								   //        set3 => 11000110 + 01100011 + 10110001 + 11011000 + 01101100 + 00110110 + 00011011 + 10001101 : 8
								   //        set4 => 10111000 .. 00010111 + 10001011 + 11000101 + 11100010 + 01110001 : 8
								   //        set5 => 10011100 .. 00100111 + 10010011 + 11001001 + 11100100 + 01110010 + 00111001 : 8
								   //        set6 => 10101010 .. 01010101 : 2 make 8
								   //        set7 => 10110100 +  01011010 + 00101101 + 10010110 + 01001011 + 10100101 + 11010010 + 01101001 : 8
								   //        set8 => 10011010 +  01001101 + 10100110 + 01010011 + 10101001 + 11010100 + 01101010 + 00110101 : 8
				{
					mask = 0xf;
				}
				else // 5 bits set0 => 11111000 01111100 00111110 00011111 10001111 11000111 11100011 11110001 : 8  // 0.6666
					 //        set1 => 10111100 : 8
					 //        set2 => 10011110 : 8
					 //        set3 => 11011100 : 8
					 //        set4 => 11001110 : 8
					 //        set5 => 11011010 : 8
					 //        set6 => 10110110 : 8
				{
					mask = 0x1F;
				}
			}
			else
			{
				if (alpha < 0.7777f) // 6 bits set0 => 11111100 01111110 00111111 10011111 11001111 11100111 11110011 11111001 : 8
								  //        set1 => 10111110 : 8
								  //        set2 => 11011110 : 8
				{
					mask = 0x3F;
				}
				else if (alpha < 0.8888f) // 7 bits set0 => 11111110 :8
				{
					mask = 0x7F;
				}
				else // all 8 bits set
					mask = 0xFF;
			}
		}

		return mask;
	}
	#endif
#endif
#endif

