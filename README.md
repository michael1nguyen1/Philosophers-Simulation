# Overview

This project simulates a classic concurrency problem known as the "Dining Philosophers Problem," where multiple philosophers sit around a table to eat spaghetti. Each philosopher needs two forks to eat, but they only have one fork each, necessitating cooperation with their neighbors to borrow forks. The philosophers follow a cycle of eating, sleeping, and thinking. If a philosopher cannot eat within a specified time, they will starve and die, causing the simulation to end.

As a bonus feature, if the philosophers manage to eat enough and become "full," the simulation ends peacefully with no fatalities, and all philosophers are content.

## Running the Simulation

To compile the program, run the following command:
```make```

After the executable is generated, you can run the simulation with 4 or 5 command-line arguments:

The 1st argument specifies the number of philosophers at the table.
The 2nd argument defines the time (in milliseconds) a philosopher can go without eating before dying.
The 3rd argument is the time (in milliseconds) it takes for a philosopher to finish eating.
The 4th argument is the time (in milliseconds) a philosopher spends sleeping.
The 5th argument (optional) sets the number of meals each philosopher must eat before they are considered full and the simulation ends.

## Examples:
```./philo 10 300 70 70 5``` In this example nobody will die and the simulation will stop when all have eaten a minimum of 5 meals.

```./philo 10 200 150 150``` Someone will die with this simulation since there is not enough time to rotate the forks.

![Screenshot from 2024-08-30 15-35-19](https://github.com/user-attachments/assets/e33b6fe9-27f5-4e58-92a1-9f9a29e4c196)
