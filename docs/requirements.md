## Functional requirements
|Function                                           | Behavior                                                                                          |
|----------------------------------------------------|------------------------------------------------------------------------------------------------|
| View Completed Orders by Driver and Time Period   | Users can view a list of completed orders for a specific driver within a specified time period. |
| View Total Mileage and Load of a Car               | Users can access the total mileage and total weight of cargo transported by a specific vehicle.  |
| View Driver's Statistics                          | Users have the capability to view the total number of trips, total weight of cargo transported, and total earnings for each driver. |
| View All Drivers' Statistics                      | The system provides an option to view comprehensive statistics for all drivers, including the total number of trips, total weight of cargo transported, and total earnings. |
| View Statistics for the Worst Performing Driver   | Users can access detailed information and total earnings for the driver who has completed the fewest trips. |
| View Information about the Car with the Highest Mileage | Users can retrieve detailed information for the vehicle with the highest total mileage. |
| View Driver's Earnings                            | Users can calculate and access the total earnings for each driver during a specified time period. |
| Store All Drivers' Earnings                       | The system automatically stores the total earnings for each driver within a specified time period in a designated table. |
| Add Car                                           | Users can add new cars to the system, providing necessary details such as the car's number, make, mileage, and load capacity. |
| Add Driver                                        | The system allows users to add new drivers, including their employee ID, name, category, experience, address, and year of birth. |
| Add Order                                         | Users have the capability to add new orders for approval, providing details such as the date, driver's name, car number, mileage, cargo weight, and transportation cost. |
| Update Car Information                           | Users can update existing car information, such as the mileage and load capacity, to maintain accurate records. |
| Update Driver Information                        | The system provides functionality to update driver information, including their name, category, experience, address, and year of birth. |
| Update Order Information                        | Users can modify existing order details, such as the date, mileage, cargo weight, and transportation cost, as needed. |
| Update Order Status                             | The system allows users to update the status of orders, such as marking them as completed or pending, to keep track of ongoing operations. |
| Update Login or Password                        | Users can update their login credentials, including the username and password, for enhanced security measures. |

### User Stories
| As a | I want to | So that | To do |
|------|-----------|---------|-------|
| Driver | View a list of completed and assigned orders within a specified time period |  Efficiently plan deliveries | Implement a function to retrieve and display a list of orders assigned to the driver. |
| Driver | Access information about my car's total mileage and total weight of cargo | Ensure the vehicle is in proper condition for transportation | Develop a function to retrieve and display the total mileage and current load of the driver's assigned vehicle. |
| Driver | View statistics related to my performance (the total number of trips, total weight of cargo transported, and total earnings) | Track productivity and efficiency | Create a function to calculate and display statistical data such as the total number of trips and weight of cargo transported by the driver. |
| Driver | Check my total earnings | Monitor earnings from completed deliveries | Implement a function to calculate and display the total amount of money earned by the driver. |
| Driver | Add orders for approval | Request new delivery tasks | Develop a function to allow the driver to submit new order requests for approval by the dispatcher. |
| Dispatcher | View a list of completed orders for a specific driver in dispatcher's city within a specified time period | Track the completed deliveries for the driver | Implement a SQL query to retrieve completed orders based on the specified driver and time period |
| Dispatcher | View the total mileage and total weight of cargo transported by a specific vehicle in dispatcher's city | Monitor the performance and usage of the vehicle | Create a SQL query to calculate the total mileage and total weight of cargo transported by the specified vehicle |
| Dispatcher | View the total number of trips, total weight of cargo transported, and total earnings for each driver in dispatcher's city | Monitor the performance and earnings of each driver | Develop a SQL query to calculate the total number of trips, total weight of cargo transported, and total earnings for each driver |
| Dispatcher | View detailed information and total earnings for the driver who has completed the fewest trips in a specific city | Assess the performance and earnings of the least active driver | Create a SQL query to retrieve detailed information and total earnings for the driver with the fewest completed trips |
| Dispatcher | View detailed information for the vehicle with the highest total mileage in a specific city| Analyze the usage and performance of the vehicle | Implement a SQL query to retrieve detailed information for the vehicle with the highest total mileage |
| Dispatcher | Develop a function to validate the weight of cargo against the carrying capacity of the vehicle before inserting a new order record into the database | Prevent exceeding the vehicle's carrying capacity and ensure safe transportation | Create a SQL function to check the weight of cargo against the vehicle's carrying capacity before inserting an order record |
| Dispatcher | Calculate the total earnings for each driver during a specified time period in a specific city and store the results in a special table | Track the earnings of drivers over time | Develop a SQL function to calculate the total earnings for each driver within the specified time period and store the results in a designated table |
| Dispatcher | Calculate the total earnings for a specified driver in a specific city during a specified time period and store the result | Track the earnings of a specific driver over time | Create a SQL function to calculate the total earnings for the specified driver within the specified time period and store the result in a designated table |
| Administrator | Implement functionality to insert, update, and delete records in the database tables | Ensure the accuracy and integrity of the data | Write SQL queries and procedures for performing CRUD operations on the database tables |
| Administrator | Use the same set of functionality as a dispatcher, but for all the regions | Manage operations across the entire system | Extend the existing dispatcher functionalities to operate across all regions by removing regional restrictions |

### Text scenarios
|  Situation  |   Scenario  |
|-------------|-------------|
| Login to the system    | Logging in is done using the username and password    |
| Login as a driver  | Get access to a limited access of functions  |
| Login as a dispatcher | Get access to a full set of functions in a specific city|
| Login as an admin | Get access to a full set of functions |
| Using available functions | Depending on the selected function, the query is made to the database and information is displayed. |
| Logging out of the user account | Allows returning to the authorization process and logging in under a different username |
| Exit | The program terminates |

## Non-functional requirements
| Category       | Description                                                                                                   |
|----------------|---------------------------------------------------------------------------------------------------------------|
| Security       | Ensures a high level of security for personal accounts by protecting users' personal data through hashing their passcodes. |
| Compatibility  | - MacOS - Linux                                                                                              |
| Reliability    | The system's code has full test coverage and meets the requirements of the assignment.                        |
| Usability      | The application is console-based and does not have a graphical interface, but the system's interface is highly user-friendly and intuitive. |
| Scalability    | The system is scalable to accommodate future growth in data volume and user load without compromising performance. |
| Performance    | The application is able to handle a large volume of data efficiently, ensuring quick response times for queries and data manipulation operations. |
| Data Integrity | Data stored in the system is accurate, consistent, and up-to-date, with appropriate validation mechanisms in place to prevent data corruption. |
| Error Handling | The system handles errors gracefully, providing informative error messages and fallback mechanisms to recover from failures. |
| Resource Efficiency | The application utilizes system resources efficiently, minimizing resource consumption and optimizing resource utilization for cost-effectiveness. |

