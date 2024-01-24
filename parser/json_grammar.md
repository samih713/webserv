- Data is presented in key/value pairs.
  - keys and values are separated by a colon ':'
- Data elements are separated by commas.
- Curly brackets {} determine objects.
  - objects can contain values, arrays and other objects
- Square brackets [] designate arrays.
  - arrays can contain values and objects

#### Value types:
  - string (always in "")
  - number
  - objects
  - arrays
  - boolean (true/false)
  - null

#### Example:
```json
{
  "className":"Class 2B",
  "year":2022,
  "phoneNumber":null,
  "active":true,
  "homeroomTeacher":{"firstName":"Richard", "lastName":"Roe"},
  "members":[{
      "firstName":"Jane","lastName":"Doe"},
      {"firstName":"Jinny","lastName":"Roe"},
      {"firstName":"Johnny","lastName":"Roe"},
      ]
}
```
Here’s what each pair indicates:
- The first line “className”:”Class 2B” is a string.
- The second pair “year”:2022 has a numeric value.
- The third pair “phoneNumber”:null represents a null – there is no value.
- The fourth pair “active”:true is a boolean expression.
- The fifth line “homeroomTeacher”:{“firstName”:”Richard”, “lastName”:”Roe”} represents an object literal.
- Lastly, the script from the sixth line onwards is an array.

#### Resources:
- https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Objects/JSON
- https://www.hostinger.com/tutorials/what-is-json