1 pip install
2 pip list

pip install -i https://pypi.tuna.tsinghua.edu.cn/simple some-package


E:\the_c_of_vs_code\python\flask>venv\scripts\activate
enter the space for python ---> venv\scripts\
-->ok (venv) E:\the_c_of_vs_code\python\flask>

1.pip install pipenv 2.flask 3.other frameworks.....
>>such as pipenv install sqlalchemy


python -m flask run  ----> run flask
python -m flask run --host=0.0.0.0 --port=80   --->change the port

python 查看项目在虚拟环境下添加的依赖包（框架）：
进入workon 虚拟空间后使用：pip3 list

how to configuration(配置) flask：
https://code.visualstudio.com/docs/python/tutorial-flask
1-> 
    In VS Code, open the Command Palette (View > Command Palette or (Ctrl+Shift+P)).
    Then select the Python: Select Interpreter command
2->
    Run Terminal: Create New Integrated Terminal (Ctrl+Shift+`)) from the Command Palette, which creates a terminal 
    and automatically activates the virtual environment
     by running its activation script
