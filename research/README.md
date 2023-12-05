# Grammar-based exhaustive testing of JSON validators   

The goal of this project is to extend the E.T. prototype to also test json validators.    
> Note this repository __only contains the code that specifically pertains to JSON exhaustive testing__ and __does not__ include any of the E.T. prototype code, as said code is not yet available in open-source.  __THE FOLLOWING WILL ONLY WORK ONCE INTEGRATED INTO E.T.__  

For this, we will be testing two json validators that have Command Line Interfaces (CLIs):  
 1. [`jsonlint`](https://github.com/prantlf/jsonlint), which is a popular online json validation tool that also has a cli.  
 2. The python `json.tool` module, which can be used as a cli using the following command `python -m json.tool <json_file>.json`.  
    
The idea would be to test both validators against each other using the following command sequence:  
```sh  
./bin/prepare_grammar json_testing/grammar/JSON.g4  
./bin/enumerate --json "jsonlint --compact;python -m json.tool" <depth>
```  

We do this by wrapping it into a harness, which generates a specific grammar based on a given width and then runs the enumeration on a given depth. The width and the depth parameters are selecting using a path walking heurstic of which we have implemented 3:  
 1. __DFS__: Increases the depth until max_depth before moving on to the next width.  
 2. __BFS__: Increases the width before enumerating over the depths and increasing it.  
 3. __RANDOM__: Randomly chooses whether to increase the width or the depth.  

The harness can be run using the following command:  
```sh
bin/harness <depth> <width> <heuristic>  
```  

  
## Installing dependencies  
For this to work, you need to have the jsonlint cli installed.  
This can be obtained through the node.js package manager (NPM), as follows (on Ubuntu):  
```sh  
# Install node.js version >= 18
sudo apt install software-properties-common  
curl -sL https://deb.nodesource.com/setup_18.x | sudo -E bash -  
sudo apt install nodejs  
 
# Install jsonlint using npm
sudo  npm i -g @prantlf/jsonlint  

# Install python packages
pip install -r requirements.txt
```  
## Understanding the output  
Both validators have similar output behaviors:  
 1. __Input is Valid__: In this case both validators output a formatted version of the input.  
 2. __Input is Invalid__: In this case both formats output an error, but in different ways:  
   - `jsonlint --compact`: the output looks as follows:  
```sh  
<json_file>.json: line <earliest_error_line>, col <error_col>, <error_msg>   

# Example  
test.json: line 21, col 5, Unexpected token }.
```    
  - `python -m json.tool`: the output looks as follows:  
```sh  
<error_msg>: line <error_line> column <error_col> (char <error char>)   

# Example  
Expecting property name enclosed in double quotes: line 21 column 5 (char 580)  
```      
  

  
