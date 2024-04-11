# Use-Case Diagram
![Use-case](https://github.com/fpmi-tp2024/tpmp-gpd-lab5-quilboars/assets/137906229/405d8de2-ad88-4794-8e13-bc6870e34a30)

# Functional requirements
| Function | Requirement |
|----------|-------------|
|Select all races | The user has the option to view a list of all the races between a specific period of time|
|Select best horse | Displays information about the horse that has the most amount of wins|
|Select Owners' horses info| Returns information about all the horses of a specific owner|
|Select most experienced jockey| Returns the jockey with the most experience|
|Select personal info| Return the personal information about the user|
|Insert horse| Inserts a horse into the system|
|Insert race record| Inserts a race record into the system|
|Insert owner| Inserts an owner into the system| 
|Insert race| Inserts a race into the system |
|Insert jockey| Inserts a race into the system |
|Update jockey information| ////// |
|Update race information| |
|Update owner information| |
|Update horse information| |
|Delete race| |
|Distribute prize| |

# Text scenarios 
## User login
**1. Precondition:** The user is on the login screen of the system<br/>
2. The user enters their username and password into the system<br/>
3. **Postcondition:** If the entered credentials are valid and the user is authorized, the system logs the user into their account and provides him with the dashboard menu.<br/>
4. **Alternative path (Invalid credentials):**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;a. If the credentials are invalid (incorrect username or password), the system displays an error message indicating the login attempt failed.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;b. The user is prompted to re-enter their credentials.<br/>

## User registration
**1. Precondition:** the user is on the registration screen of the system. <br/>
2. The user enters the required registration information, including email and password.<br/>
**3.Postcondition:** If the entered information is valid and meets the system's requirements the user is granted access to their new account.<br/>
**4. Alternative path (Existing email):** <br/>
&nbsp;&nbsp;&nbsp;&nbsp;a. If the entered email address is already associated with an existing account, the 
system displays an error message indicating that the email address is already in use.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;b. The user is prompted to  use a different email address for registration.<br/>
**5. Alternative path (Weak password):**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;a. If the entered password does not meet the system's password strength requirements (minimum 8 characters), the system displays an error message indicating that the password is weak.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;b. The user is prompted to choose a stronger password that meets the specified requirements.<br/>

## Using a function in the system
**1. Precondition:** The user is logged into their account on the system.<br/>
2. The user selects the option to use a function in the system (select, delete, update, insert, give prize)<br/>
4. The system prompts the user with additional information about the operation (e.g. id of horse, prize amount)<br/>
5. The user enters the values for the additional information<br/>
**3. Postcondition:** If the entered data is valid and meets the database constraints the system preforms the operation.<br/>
**4. Alternative path (Invalid data format):** <br/>
&nbsp;&nbsp;&nbsp;&nbsp;a. If the user enters data in an incorrect format or violates any data constraints (e.g., entering non existing data, invalid data type), the system displays an error message.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;b. The user is prompted to correct the data format or values before redoing the operation.<br/>


