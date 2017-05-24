#include "textual.h"


void Textual(trip_node *headtrip, station_node *headstation, int trip_count, int station_count, int max_id)
{
	int option = 0, flag = 0;

	trip_node *copytrip = headtrip;
	station_node *copystation = headstation;

	printf("headtrip %d\n", headtrip->payload.tripID);

	printf("%d\n", headstation->payload.stationID);

	while(option != -1)
	{
		//if(option != 0) scanf("%d", &option);

		switch(option)
		{
			case 0:
				MainMenu(&option);
				break;

			case 1:
				DataSelection(&copytrip, &copystation, headtrip, headstation, &flag);
				option = 0;
				break;

			case 2:
				printf("%d\n", copytrip->payload.tripID);
				TripListing(copytrip);
				option = 0;
				break;

			case 3:
				StationListing(copytrip, copystation, station_count, max_id);
				option = 0;
				break;

			case 4:
				RouteListing();
				option = 0;
				break;

			case 5:
				StatsListing();
				option = 0;
				break;

			case 6:
				option = -1;
				break;

			default:
				break;
		}
	}
}

void MainMenu(int *option)
{
	int aux = 0;

	printf("Escolha a opção que deseja:\n\n");

	printf("\t1. Selecção de dados\n\t2. Listagem de viagens\n\t3. Listagem de estações\n\t4. Listagem de rotas\n\t5. Listagem de estatísticas\n\t6. Exit\n");

	while(*option < 1 || *option > 6)
	{
		if(aux == 1) printf("Escolha inválida\n");

		scanf("%d", &(*option));

		aux = 1;
	}
}

void DataSelection(trip_node **copytrip, station_node **copystation, trip_node *headtrip, station_node *headstation, int *flag)
{
	int pri_option = 0, sec_option = 0, aux = 0, start_hours = 0, start_minutes = 0, end_hours = 0, end_minutes = 0;

	char selection[MAX_STRING];

	selection[0] = '\0';


	while(((sscanf(selection,"%d", &pri_option) != 1) || (pri_option < 1) || (pri_option > 5)) && (pri_option != 4) && (pri_option != 5)) 
	{

		if(aux == 1) 
		{
			printf("\nEscolha inválida\n\n");
		}

		getchar();

		printf("Escolha o(s) parâmetros que deseja (insira 5 para sair):\n");
		printf("\t(escreva 'del <número do parâmetro>' para retirar esse parâmetro)\n\n");

		printf("\t1. Período de tempo:\n\n\t2. Dia da semana\n\n\t3. Duração\n\n\t4. Remover restrições\n\n\t5. Sair para o menu principal\n");

		fgets(selection, MAX_STRING, stdin);

		aux = 1;
	}

	switch(pri_option)
	{
		case 1:
			// code for period of time selection

			while((sscanf(selection,"%d:%d %d:%d", &start_hours, &start_minutes, &end_hours, &end_minutes) != 4) || (start_hours < 0) || (start_minutes < 0) || (end_hours < 0) || (end_minutes < 0))
			{
				if(aux == 1) 
				{
					printf("\nEscolha inválida\n\n");
				}

				getchar();

				printf("\t-> to add: <start hour>:<start minute> <end hour>:<end minute>\n");

				fgets(selection, MAX_STRING, stdin);

				aux = 1;
			}

			printf("%d %d %d %d %d\n", pri_option, start_hours, start_minutes, end_hours, end_minutes);

			TimePeriod(&(*copytrip), &(*copystation), start_hours, start_minutes, end_hours, end_minutes, *flag);
			*flag = 1;
			break;

		case 2:
			// code for day of the week selection

			while((sscanf(selection,"%d", &sec_option) != 1) || (sec_option < 0) || (sec_option > 7))
			{
				if(aux == 1)
				{
					printf("\nEscolha inválida\n\n");
				}

				getchar();

				printf("\t-> to add: <dia da semana>\n");

				fgets(selection, MAX_STRING, stdin);

				aux = 1;
			}

			WeekDay(&(*copytrip), sec_option, *flag);
			*flag = 2;
			break;

		case 3:
			// code for duration selection

			while((sscanf(selection,"%d", &sec_option) != 1) || (sec_option < 0))
			{
				if(aux == 1)
				{
					printf("\nEscolha inválida\n\n");
				}

				getchar();

				printf("\t-> to add: 3 <duração>\n");

				fgets(selection, MAX_STRING, stdin);

				aux = 1;
			}

			TripDuration(&(*copytrip), sec_option, *flag);
			*flag = 3;
			break;

		case 4:
			// code to remove restrictions
			if(*flag != 0) RemoveSelections(&(*copytrip), headtrip);

		default:
			break;
	}
}

void TimePeriod(trip_node **copytrip, station_node **copystation, int start_hours, int start_minutes, int end_hours, int end_minutes, int flag)
{
	//percorrer listas para copiar nós

	int k = 0, last = 0, end = 0;

	trip_node *T_head = (*copytrip), *T_aux = NULL, *T_current = NULL;
	//station_node *S_head = NULL, *S_aux = NULL, *S_current = NULL;

	if(flag == 0)
	{
		while((*copytrip) != NULL)
		{

			if((*copytrip)->payload.end_time.hours < (*copytrip)->payload.start_time.hours)
			{
				if((*copytrip)->payload.end_time.minutes < (*copytrip)->payload.start_time.minutes)
				{
					last = (*copytrip)->payload.end_time.hours + 24;

					end = end_hours + 24;
				}
			}
			else
			{
				last = (*copytrip)->payload.end_time.hours;

				end = end_hours;
			}



			if(((*copytrip)->payload.start_time.hours >= start_hours) && (last <= end))
			{
				if(((*copytrip)->payload.start_time.minutes >= start_minutes) && ((*copytrip)->payload.end_time.minutes <= end_minutes))
				{

					printf("%d %d %d %d\n", (*copytrip)->payload.start_time.hours, start_hours, last, end);
					
					T_current = (trip_node *) malloc(sizeof(trip_node));

					if(T_current == NULL)
					{
						printf("Erro na alocação de memória.\n");
						exit(EXIT_FAILURE);
					}

					if(T_aux == NULL)
					{
						T_head = T_current;

						T_aux = T_current;
					}
					else
					{
						T_aux->next = T_current;

						T_aux = T_current;
					}
					//copiar valores

					T_current->payload.tripID = (*copytrip)->payload.tripID;


					T_current->payload.duration = (*copytrip)->payload.duration;


					T_current->payload.start_date.month = (*copytrip)->payload.start_date.month;

					T_current->payload.start_date.day = (*copytrip)->payload.start_date.day;

					T_current->payload.start_date.year = (*copytrip)->payload.start_date.year;


					T_current->payload.start_time.hours = (*copytrip)->payload.start_time.hours;

					T_current->payload.start_time.minutes = (*copytrip)->payload.start_time.minutes;

					
					T_current->payload.end_date.month = (*copytrip)->payload.end_date.month;

					T_current->payload.end_date.day = (*copytrip)->payload.end_date.day;

					T_current->payload.end_date.year = (*copytrip)->payload.end_date.year;


					T_current->payload.end_time.hours = (*copytrip)->payload.end_time.hours;

					T_current->payload.end_time.minutes = (*copytrip)->payload.end_time.minutes;


					T_current->payload.start_stationID = (*copytrip)->payload.start_stationID;

					T_current->payload.end_stationID = (*copytrip)->payload.end_stationID;


					strcpy(T_current->payload.bikeID, (*copytrip)->payload.bikeID);


					T_current->payload.user.membership = (*copytrip)->payload.user.membership;

					T_current->payload.user.birth = (*copytrip)->payload.user.birth;

					T_current->payload.user.gender = (*copytrip)->payload.user.gender;


					T_current->next = NULL;

					T_current = T_current->next;
				}
			}
			

			
			(*copytrip) = (*copytrip)->next;
		}
	}
	else
	{
		while((*copytrip) != NULL)
		{

			if(end_hours < start_hours)
			{
				if(end_minutes < start_minutes)
				{
					last = (*copytrip)->payload.end_time.hours + 24;

					end = end_hours + 24;
				}
			}
			else
			{
				last = (*copytrip)->payload.end_time.hours;

				end = end_hours;
			}

			if(((*copytrip)->payload.start_time.hours < start_hours) || (last > end))
			{
				if(((*copytrip)->payload.start_time.minutes < start_minutes) || ((*copytrip)->payload.end_time.minutes > end_minutes))
				{
					T_aux = (*copytrip);

					(*copytrip) = (*copytrip)->next;

					free(T_aux);
				}
			}
			
		}
	}


	(*copytrip) = T_head;



	/*while((*copystation) != NULL)
	{

		if(S_aux == NULL)
		{
			S_head = S_current;

			S_aux = S_current;
		}
		else
		{
			S_aux->next = S_current;

			S_aux = S_current;
		}


		S_current = (station_node *) malloc(sizeof(station_node));

		//copiar valores

		S_current->payload.stationID = (*copystation)->payload.stationID;


		strcpy(S_current->payload.terminal, (*copystation)->payload.terminal);

		strcpy(S_current->payload.long_name, (*copystation)->payload.long_name);

		strcpy(S_current->payload.municipal, (*copystation)->payload.municipal);

		

		S_current->payload.place.latitude = (*copystation)->payload.place.latitude;

		S_current->payload.place.longitude = (*copystation)->payload.place.longitude;


		S_current->payload.status = (*copystation)->payload.status;



		S_current->next = NULL;

		S_current = S_current->next;



		(*copystation) = (*copystation)->next;
	}*/
	//copystation = S_head;


}

void WeekDay(trip_node **copytrip, int parameter, int flag)
{
	trip_node *T_head = (*copytrip), *T_aux = NULL, *T_current = NULL;
	//station_node *S_head = NULL, *S_aux = NULL, *S_current = NULL;

	if(flag == 0)
	{
		while((*copytrip) != NULL)
		{

			if(CalculateWday((*copytrip)->payload.start_date.month, (*copytrip)->payload.start_date.day, (*copytrip)->payload.start_date.year) == parameter /*|| CalculateWday((*copytrip)->payload.end_date.month, (*copytrip)->payload.end_date.day, (*copytrip)->payload.end_date.year) == parameter*/)
			{

				T_current = (trip_node *) malloc(sizeof(trip_node));

				if(T_current == NULL)
				{
					printf("Erro na alocação de memória.\n");
					exit(EXIT_FAILURE);
				}

				if(T_aux == NULL)
				{
					T_head = T_current;

					T_aux = T_current;
				}
				else
				{
					T_aux->next = T_current;

					T_aux = T_current;
				}

				//copiar valores

				T_current->payload.tripID = (*copytrip)->payload.tripID;


				T_current->payload.duration = (*copytrip)->payload.duration;


				T_current->payload.start_date.month = (*copytrip)->payload.start_date.month;

				T_current->payload.start_date.day = (*copytrip)->payload.start_date.day;

				T_current->payload.start_date.year = (*copytrip)->payload.start_date.year;


				T_current->payload.start_time.hours = (*copytrip)->payload.start_time.hours;

				T_current->payload.start_time.minutes = (*copytrip)->payload.start_time.minutes;

					
				T_current->payload.end_date.month = (*copytrip)->payload.end_date.month;

				T_current->payload.end_date.day = (*copytrip)->payload.end_date.day;

				T_current->payload.end_date.year = (*copytrip)->payload.end_date.year;


				T_current->payload.end_time.hours = (*copytrip)->payload.end_time.hours;

				T_current->payload.end_time.minutes = (*copytrip)->payload.end_time.minutes;


				T_current->payload.start_stationID = (*copytrip)->payload.start_stationID;

				T_current->payload.end_stationID = (*copytrip)->payload.end_stationID;


				strcpy(T_current->payload.bikeID, (*copytrip)->payload.bikeID);


				T_current->payload.user.membership = (*copytrip)->payload.user.membership;

				T_current->payload.user.birth = (*copytrip)->payload.user.birth;

				T_current->payload.user.gender = (*copytrip)->payload.user.gender;


				T_current->next = NULL;

				T_current = T_current->next;

			}


			(*copytrip) = (*copytrip)->next;
		}
	}
	else
	{
		while((*copytrip) != NULL)
		{
			if(CalculateWday((*copytrip)->payload.start_date.month, (*copytrip)->payload.start_date.day, (*copytrip)->payload.start_date.year) != parameter && CalculateWday((*copytrip)->payload.end_date.month, (*copytrip)->payload.end_date.day, (*copytrip)->payload.end_date.year) != parameter)
			{
				T_aux = (*copytrip);

				(*copytrip) = (*copytrip)->next;

				free(T_aux);
			}
		}
	}

	(*copytrip) = T_head;
}

void TripDuration(trip_node **copytrip, int parameter, int flag)
{
	trip_node *T_head = (*copytrip), *T_aux = NULL, *T_current = NULL;
	//station_node *S_head = NULL, *S_aux = NULL, *S_current = NULL;

	if(flag == 0)
	{
		while((*copytrip) != NULL)
		{

			if((*copytrip)->payload.duration <= parameter && (*copytrip)->payload.duration >= 0)
			{


				T_current = (trip_node *) malloc(sizeof(trip_node));

				if(T_current == NULL)
				{
					printf("Erro na alocação de memória.\n");
					exit(EXIT_FAILURE);
				}

				if(T_aux == NULL)
				{
					T_head = T_current;

					T_aux = T_current;
				}
				else
				{
					T_aux->next = T_current;

					T_aux = T_current;
				}

				//copiar valores

				T_current->payload.tripID = (*copytrip)->payload.tripID;


				T_current->payload.duration = (*copytrip)->payload.duration;


				T_current->payload.start_date.month = (*copytrip)->payload.start_date.month;

				T_current->payload.start_date.day = (*copytrip)->payload.start_date.day;

				T_current->payload.start_date.year = (*copytrip)->payload.start_date.year;


				T_current->payload.start_time.hours = (*copytrip)->payload.start_time.hours;

				T_current->payload.start_time.minutes = (*copytrip)->payload.start_time.minutes;

					
				T_current->payload.end_date.month = (*copytrip)->payload.end_date.month;

				T_current->payload.end_date.day = (*copytrip)->payload.end_date.day;

				T_current->payload.end_date.year = (*copytrip)->payload.end_date.year;


				T_current->payload.end_time.hours = (*copytrip)->payload.end_time.hours;

				T_current->payload.end_time.minutes = (*copytrip)->payload.end_time.minutes;


				T_current->payload.start_stationID = (*copytrip)->payload.start_stationID;

				T_current->payload.end_stationID = (*copytrip)->payload.end_stationID;


				strcpy(T_current->payload.bikeID, (*copytrip)->payload.bikeID);


				T_current->payload.user.membership = (*copytrip)->payload.user.membership;

				T_current->payload.user.birth = (*copytrip)->payload.user.birth;

				T_current->payload.user.gender = (*copytrip)->payload.user.gender;


				T_current->next = NULL;

				T_current = T_current->next;

			}


			(*copytrip) = (*copytrip)->next;
		}
	}
	else
	{
		while((*copytrip) != NULL)
		{
			if((*copytrip)->payload.duration > parameter || (*copytrip)->payload.duration < 0)
			{
				T_aux = (*copytrip);

				(*copytrip) = (*copytrip)->next;

				free(T_aux);
			}
		}
	}

	(*copytrip) = T_head;
}

void RemoveSelections(trip_node **copytrip, trip_node *headtrip)
{
	trip_node *aux = (*copytrip);

	while(aux != NULL)
	{
		(*copytrip) = (*copytrip)->next;

		free(aux);

		aux = (*copytrip);		
	}

	(*copytrip) = headtrip;
}


void TripListing(trip_node *headtrip)
{
	char str_aux[MAX_STRING];

	str_aux[0] = '\0';

	int trip_num, mult = 0, i;

	trip_node *trips;

	trips = headtrip;

	printf("Quantas viagens a apresentar de cada vez?\n(é possível navegar pelas páginas inserindo \"n\" para ver a próxima, \"p\" para ver a anterior e \"exit\" para sair da listagem para o menu principal)\n");

	while(scanf("%d", &trip_num) != 1 && trip_num < 0)
	{
		printf("Número de viagens inválido.\n");
	}

	getchar();

	while(strcmp(str_aux, "exit\n") != 0)
	{

		for(i = 0; i < mult*trip_num && headtrip != NULL && headtrip->next != NULL; i++)
		{
			headtrip = headtrip->next;
		}

		for(i = 0; i < trip_num && headtrip != NULL; i++)
		{
			if(headtrip != NULL) printf("tripID = %d, duration = %d, %d/%d/%d %d:%d, %d/%d/%d %d:%d\n", headtrip->payload.tripID, headtrip->payload.duration, headtrip->payload.start_date.month, headtrip->payload.start_date.day, headtrip->payload.start_date.year, headtrip->payload.start_time.hours, headtrip->payload.start_time.minutes, headtrip->payload.end_date.month, headtrip->payload.end_date.day, headtrip->payload.end_date.year, headtrip->payload.end_time.hours, headtrip->payload.end_time.minutes);
			else printf("Está a NULL.\n");

			headtrip = headtrip->next;
		}

		fgets(str_aux, MAX_STRING, stdin);

		if(strcmp(str_aux,"n\n") == 0 && headtrip != NULL && headtrip->next != NULL) mult++;
		if(strcmp(str_aux,"p\n") == 0 && mult > 0) mult--;

		headtrip = trips;
	}


}

void StationListing(trip_node *copytrip, station_node *copystation, int station_count, int max_id)
{
	int ***bikes = NULL, max = 0, min = 0, aver = 0;

	// 0 for departure and 1 for arrival
	max_id++;

	bikes = (int ***) calloc(max_id, sizeof(int**));

	for(int i = 0; i < max_id; i++)
	{
		bikes[i] = (int **) calloc(24, sizeof(int *));
	}

	for(int i = 0; i < max_id; i++)
	{
		for(int j = 0; j < 24; j++)
		{
			bikes[i][j] = (int *) calloc(2, sizeof(int));
		}
	}

	while(copytrip != NULL)
	{
		bikes[copytrip->payload.start_stationID][copytrip->payload.start_time.hours][0] += 1;
		bikes[copytrip->payload.end_stationID][copytrip->payload.end_time.hours][1] += 1;
		
		copytrip = copytrip->next;
	}

	while(copystation != NULL)
	{
		printf("%d, %s, (%f;%f)\n", copystation->payload.stationID, copystation->payload.long_name, copystation->payload.place.latitude, copystation->payload.place.longitude);

		for()
		{

		}
		printf("max = %d, min = %d, aver = %d", , , );

		copystation = copystation->next;
	}

	
	for(int i = 0; i < max_id; i++)
	{
		for(int j = 0; j < 24; j++)
		{
			free(bikes[i][j]);
		}
	}

	for(int i = 0; i < max_id; i++)
	{
		free(bikes[i]);
	}

	free(bikes);

}

void RouteListing()
{

}

void StatsListing()
{

}


