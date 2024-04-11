# Assignment

The car park provides services for transporting goods, using its own fleet of cars and drivers for this purpose. A driver who completes an order receives 20% of the transportation cost.

Car park management possesses information about:
- Cars: car number, brand, mileage at the time of purchase, carrying capacity.
- Drivers: employee ID, driver's last name, category, experience, address, year of birth.
- Completed orders: date, driver's last name, car number, mileage, cargo weight, transportation cost.

A car park driver can access information about their own car, only their data - see the section marked with an asterisk (*).

The following tasks need to be accomplished:
1. Create database tables considering data integrity constraints.
2. Use the Select operator to retrieve the following information:
   - For a specified driver: a list of completed orders for a specified period (*).
   - For a specified car: total mileage and total weight of transported goods (*).
   - For each driver: total number of trips, total weight of transported goods, total earnings (*).
   - For the driver who completed the fewest trips: all details and the amount earned.
   - For the car with the highest total mileage: all details (*).
3. Ensure data updating in the specified tables using Insert, Update, Delete operators.
4. Create a function that, when adding information to the orders table, checks if the cargo weight exceeds the car's carrying capacity, and if so, prevents inserting information into the table (*).
5. Create a function that determines the amount of money earned by each driver for transportation over a specified period. Pass the initial date and end date of the period as parameters. Store the results in a special table (*).
6. Create a function that determines the amount of money earned by a specified driver for transportation over a specified period. Pass the initial date, end date, and driver's last name as parameters (*).
