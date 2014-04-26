uint16_t courbe[128][2];
	float pente_courbe[127];		// pente_courbe(0) = pente entre 0 et 1
	uint8_t index = 0, index_max;
	uint8_t i;
	float pente;
	
	
	
	courbe[0][0] = 0;
	courbe[0][1] = camera2_valeurs_m[0];

	courbe[1][0] = 1;
	courbe[1][1] = camera2_valeurs_m[1];
	pente_courbe[0] = (float)(courbe[1][1] - courbe[0][1]);
	
	index = 1;
	i = 2;
	
	while(i < 128)
	{
		pente = (camera2_valeurs_m[i] - courbe[index][1]) * un_sur[i - courbe[index][0]];
		
		if (index != 0)
		{
			if(pente < pente_courbe[index - 1])
			{
				
				pente_courbe[index] = pente;
				index++;
				courbe[index][0] = i;
				courbe[index][1] = camera2_valeurs_m[i];
				i++;
			}
			else
			{
				index--;
			}
		}
		else
		{
			
			pente_courbe[0] = pente;
			index++;
			courbe[1][0] = i;
			courbe[1][1] = camera2_valeurs_m[i];
			i++;
		}
		
	}
	
	i = 0;
	index_max = index;
	index = 0;
	while (index < index_max)
	{
		while (i < courbe[index + 1][0])
		{
			camera2_courbe[i] = courbe[index][1] + (int16_t)((i - courbe[index][0]) * pente_courbe[index]);
			i++;
		}
		index++;
	}
	camera2_courbe[127] = courbe[index_max][1];