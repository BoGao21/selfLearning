

import os

#创建数据源，创建写入操作执行一次即可
os.makedirs(os.path.join('.', 'data'), exist_ok=True)
data_file = os.path.join('.', 'data', 'house_tiny.csv')
with open(data_file, 'w') as f:
    f.write('NumRooms,Alley,Price\n')  # 列名
    f.write('NA,Pave,127500\n')  # 每行表示一个数据样本
    f.write('2,NA,106000\n')
    f.write('4,House,178100\n')
    f.write('NA,NA,140000\n')

import pandas as pd

data = pd.read_csv(data_file)
print(data)

#判断每个元素是否为无效值
print(f"na={data.isna()}")

#data.iloc表示的是csv中的一行数据，data.iloc[0]表示第一行NA,Pave,127500,这个数据是多个，所以也是个列表
#inputs 是 pandas.DataFrame 类型
#因为 data.iloc[:, 0:2] 选取了前两列（多列），返回一个二维表格结构。
#outputs 是 pandas.Series 类型
#因为 data.iloc[:, 2] 只选取了第三列（单列），返回一维序列（带索引）
inputs, outputs = data.iloc[:, 0:2], data.iloc[:, 2]
print(f"inputs:\n{inputs}")
print(f"output:\n{outputs}")

#处理数值型缺失值，使用.mean()操作
if 0:
    #当你在pandas.Series类型数据，比如inputs['NumRooms'] 这样的单列上调用时，它返回一个浮点数（平均值）
    inputs["NumRooms"] = inputs["NumRooms"].fillna(inputs["NumRooms"].mean())
    print(f"inputs(series):\n{inputs}")
else:
    #当你在pandas.DataFrame类型数据上调用时，它返回一个新的 Series（索引是列名，值是每列的平均值，就是每列求均值填充）。但前提是：所有列必须都是数值类型，或者你显式指定只计算数值列，就是指定numeric_only=True
    inputs = inputs.fillna(inputs.mean(numeric_only=True))
    print(f"inputs(dataframe):\n{inputs}")

#处理字符型缺失值，使用独热编码One-Hot Encoding
#核心作用就是将表格中的文本类别（如 Alley 列）转换成机器学习模型能处理的数字（0 和 1），方法就是对文本进行分类，然后根据值，给出0/1
#在深度学习中，线性代数运算（矩阵乘法）只认数字。不把 Pave 这种字符串转成 0/1，模型就无法训练。而加上 dummy_na=True 是为了不丢弃含有缺失值的样本，把“缺失”也变成了一个特征维度。
#对于inputs中的类别值或离散值，我们将“NaN”视为一个类别。 由于“巷子类型”（“Alley”）列只接受两种类型的类别值“Pave”和“NaN”， pandas可以自动将此列转换为两列“Alley_Pave”和“Alley_nan”。 巷子类型为“Pave”的行会将“Alley_Pave”的值设置为1，“Alley_nan”的值设置为0。 缺少巷子类型的行会将“Alley_Pave”和“Alley_nan”分别设置为0和1。——这是原始解释，代码我改为三种文本空间，上面帮助理解即可
inputs = pd.get_dummies(inputs, dummy_na=True)
print(f"inputs:\n{inputs}")

#转为张量格式
import torch

X = torch.tensor(inputs.to_numpy(dtype=float))
Y = torch.tensor(outputs.to_numpy(dtype=float))
print(f"X={X}")
print(X.shape)
print(f"Y={Y}")
print(Y.shape)

```
运行结果：
   NumRooms  Alley   Price
0       NaN   Pave  127500
1       2.0    NaN  106000
2       4.0  House  178100
3       NaN    NaN  140000
na=   NumRooms  Alley  Price
0      True  False  False
1     False   True  False
2     False  False  False
3      True   True  False
inputs:
   NumRooms  Alley
0       NaN   Pave
1       2.0    NaN
2       4.0  House
3       NaN    NaN
output:
0    127500
1    106000
2    178100
3    140000
Name: Price, dtype: int64
inputs(dataframe):
   NumRooms  Alley
0       3.0   Pave
1       2.0    NaN
2       4.0  House
3       3.0    NaN
inputs:
   NumRooms  Alley_House  Alley_Pave  Alley_nan
0       3.0        False        True      False
1       2.0        False       False       True
2       4.0         True       False      False
3       3.0        False       False       True
X=tensor([[3., 0., 1., 0.],
        [2., 0., 0., 1.],
        [4., 1., 0., 0.],
        [3., 0., 0., 1.]], dtype=torch.float64)
torch.Size([4, 4])
Y=tensor([127500., 106000., 178100., 140000.], dtype=torch.float64)
torch.Size([4])
```
