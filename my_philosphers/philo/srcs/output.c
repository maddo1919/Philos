
#include "philo.h"

static void	print_status_debug(t_philo *philo, char *color,
								char *str, t_status status)
{
	time_t timestamp = get_time_in_ms() - philo->table->start_time;
	if (status == GOT_FORK_1)
		printf("[time %10ld]  %sPhilosopher %d  %-20s%s  [fork %d]\n",
			timestamp, color, philo->id + 1, str, NC, philo->fork[0]);
	else if (status == GOT_FORK_2)
		printf("[time %10ld]  %sPhilosopher %d  %-20s%s  [fork %d]\n",
			timestamp, color, philo->id + 1, str, NC, philo->fork[1]);
	else
		printf("[time %10ld]  %sPhilosopher %d  %-20s%s\n",
			timestamp, color, philo->id + 1, str, NC);
}

static void	write_status_debug(t_philo *philo, t_status status)
{
	if (status == DIED)
		print_status_debug(philo, RED, "died", status);
	else if (status == EATING)
		print_status_debug(philo, GREEN, "is eating", status);
	else if (status == SLEEPING)
		print_status_debug(philo, CYAN, "is sleeping", status);
	else if (status == THINKING)
		print_status_debug(philo, CYAN, "is thinking", status);
	else if (status == GOT_FORK_1)
		print_status_debug(philo, PURPLE, "has taken a fork", status);
	else if (status == GOT_FORK_2)
		print_status_debug(philo, PURPLE, "has taken a fork", status);
}

static void	print_status(t_philo *philo, char *str)
{
	printf("%ld %d %s\n", get_time_in_ms() - philo->table->start_time,
		philo->id + 1, str);
}

	
void	write_status(t_philo *philo, bool reaper_report, t_status status)
{	
	pthread_mutex_lock(&philo->table->write_lock);
	if (has_simulation_stopped(philo->table) == true && reaper_report == false)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (DEBUG_FORMATTING == true)
	{
		write_status_debug(philo, status);
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (status == DIED)
		print_status(philo, "died");
	else if (status == EATING)
		print_status(philo, "is eating");
	else if (status == SLEEPING)
		print_status(philo, "is sleeping");
	else if (status == THINKING)
		print_status(philo, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->table->write_lock);
}

void	write_outcome(t_table *table)
{
	unsigned int	i;
	unsigned int	full_count;

	full_count = 0;
	i = 0;
	while (i < table->nb_philos)
	{
		if (table->philos[i]->times_ate >= (unsigned int)table->must_eat_count)
			full_count++;
		i++;
	}
	pthread_mutex_lock(&table->write_lock);
	printf("%d/%d philosophers had at least %d meals.\n",
		full_count, table->nb_philos, table->must_eat_count);
	pthread_mutex_unlock(&table->write_lock);
	return ;
}
