void derivee (uint8_t* pos_max, uint8_t* pos_min)
{
    *pos_max = 0;
    *pos_min = 0;
    
    // calcul de la dérivée et, recherche du min et du max
    for(i = 0; i < 126; i++)
   	{
        valeurs[i] = (int8_t)camera_valeurs[i + 2] - (int8_t)camera_valeurs[i];
        if(valeurs[i] > valeurs[*pos_max])
			*pos_max = i;
		if(valeurs[i] < valeurs[*pos_min])
			*pos_min = i;
		
    }
}