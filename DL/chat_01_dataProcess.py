```
学习pytorch中tensor张量的基本操作
对应教材https://zh.d2l.ai/chapter_preliminaries/ndarray.html
```
import torch

#张量的定义
x = torch.arange(8)
print(x)
print(x.shape) #这里是成员，返回的是torch.Size
print(x.numel()) #这里是函数，求张量中元素总数，返回的是数字

x = x.reshape(2,4) #改变一个张量的形状而不改变元素数量和元素值
print(x)
print(x.shape) #这里是成员，返回的是torch.Size
print(x.numel()) #这里是函数，求张量中元素总数，返回的是数字

x = x.reshape(4,-1) #若张量的另一个维度可以明确算出，可以用-1替代，方法内部会自动算，不需要我们指定
print(x)
print(x.shape) #这里是成员，返回的是torch.Size
print(x.numel()) #这里是函数，求张量中元素总数，返回的是数字

x = torch.zeros((2,3,4)) #创建元素全为0的向量
print(x)
print(x.shape) #这里是成员，返回的是torch.Size
print(x.numel()) #这里是函数，求张量中元素总数，返回的是数字

x = torch.ones((2,3,4)) #创建元素全为1的向量
print(x)
print(x.shape) #这里是成员，返回的是torch.Size
print(x.numel()) #这里是函数，求张量中元素总数，返回的是数字

x = torch.randn((3,4)) #创建元素为随机值的向量
print(x)
print(x.shape) #这里是成员，返回的是torch.Size
print(x.numel()) #这里是函数，求张量中元素总数，返回的是数字

x = torch.tensor([[1,1,1],[2,2,2],[3,3,3]])
print(x)
print(x.shape) #这里是成员，返回的是torch.Size
print(x.numel()) #这里是函数，求张量中元素总数，返回的是数字

x = torch.tensor([[[1,1,1],[2,2,2],[3,3,3]],[[-1,-1,-1],[-2,-2,-2],[-3,-3,-3]]])
print(x)
print(x.shape) #这里是成员，返回的是torch.Size
print(x.numel()) #这里是函数，求张量中元素总数，返回的是数字


#张量的计算
x1 = torch.tensor([[1,2,4],[8,4,5]])
x2 = torch.tensor([[6,34,2],[9,21,55]])
print(x1)
print(x2)

print(f"x1 + x2 = {x1 + x2}")
print(f"x1 - x2 = {x1 - x2}")
print(f"x1 * x2 = {x1 * x2}")
print(f"x1 / x2 = {x1 / x2}")
print(f"x1 ** x2 = {x1 ** x2}")
print(f"exp(x1) = {torch.exp(x1)}")
print(f"exp(x2) = {torch.exp(x2)}")


x1 = torch.arange(18,dtype=float).reshape(2,3,3)
x2 = torch.tensor([[[1,2,3],[4,5,6],[7,8,9]],[[10,11,12],[13,14,15],[16,17,18]]])
print(x1)
print(x1.shape)
print(x2)
print(x2.shape)

print("tensor operation cat res:")
res = torch.cat((x1,x2),dim=0)
print(res)
print(res.shape)

res = torch.cat((x1,x2),dim=1)
print(res)
print(res.shape)

res = torch.cat((x1,x2),dim=2)
print(res)
print(res.shape)

print(f"x1 == x2 : {x1 == x2}")
print(f"x1.sum = {x1.sum()}")
print(f"x2.sum = {x2.sum()}")


#广播机制
#即使形状不同，我们仍然可以通过调用 广播机制（broadcasting mechanism）来执行按元素操作。 这种机制的工作方式如下：
#1.通过适当复制元素来扩展一个或两个数组，以便在转换之后，两个张量具有相同的形状；
#2.对生成的数组执行按元素操作。
#扩维是通过复制当前元素扩充维度，扩充到的维度是取每个维度的最大值

a = torch.arange(3).reshape((3, 1))
b = torch.arange(2).reshape((1, 2))
print(f"a={a}")
print(f"b={b}")
print(f"a+b={a+b}")
#a:[[0],[1],[2]] -> [[0,0],[1,1],[2,2]]
#b:[[0,1]] -> [[0,1],[0,1],[0,1]]


#索引访问
x = torch.arange(9).reshape(3,3)
print(x)
print(f"x[1]={x[1]}")
print(f"x[0:2]={x[0:2]}")
print(f"x[-1]={x[-1]}")
x[1,2] = 99
print(f"x[1,2] = 99, res = {x}")
x[0:2, 2] = 199
print(f"x[0:2,2] = 199, res = {x}")
x[2, 0:2] = 299
print(f"x[2,0:2] = 299, res = {x}")
x[0,:] = 888
print(f"x[0,:] = 888, res = {x}")

#节省内存
#使用切片原地更新张量
print(id(x))
x = x + 1
print(id(x))
x[:] = x + 2
print(id(x))


```
执行结果：
tensor([0, 1, 2, 3, 4, 5, 6, 7])
torch.Size([8])
8
tensor([[0, 1, 2, 3],
        [4, 5, 6, 7]])
torch.Size([2, 4])
8
tensor([[0, 1],
        [2, 3],
        [4, 5],
        [6, 7]])
torch.Size([4, 2])
8
tensor([[[0., 0., 0., 0.],
         [0., 0., 0., 0.],
         [0., 0., 0., 0.]],

        [[0., 0., 0., 0.],
         [0., 0., 0., 0.],
         [0., 0., 0., 0.]]])
torch.Size([2, 3, 4])
24
tensor([[[1., 1., 1., 1.],
         [1., 1., 1., 1.],
         [1., 1., 1., 1.]],

        [[1., 1., 1., 1.],
         [1., 1., 1., 1.],
         [1., 1., 1., 1.]]])
torch.Size([2, 3, 4])
24
tensor([[-1.2170, -0.0339,  0.0228, -0.5724],
        [-1.3565, -0.9072, -0.4838,  3.3537],
        [ 1.3881,  0.5803,  0.2723, -0.1873]])
torch.Size([3, 4])
12
tensor([[1, 1, 1],
        [2, 2, 2],
        [3, 3, 3]])
torch.Size([3, 3])
9
tensor([[[ 1,  1,  1],
         [ 2,  2,  2],
         [ 3,  3,  3]],

        [[-1, -1, -1],
         [-2, -2, -2],
         [-3, -3, -3]]])
torch.Size([2, 3, 3])
18
tensor([[1, 2, 4],
        [8, 4, 5]])
tensor([[ 6, 34,  2],
        [ 9, 21, 55]])
x1 + x2 = tensor([[ 7, 36,  6],
        [17, 25, 60]])
x1 - x2 = tensor([[ -5, -32,   2],
        [ -1, -17, -50]])
x1 * x2 = tensor([[  6,  68,   8],
        [ 72,  84, 275]])
x1 / x2 = tensor([[0.1667, 0.0588, 2.0000],
        [0.8889, 0.1905, 0.0909]])
x1 ** x2 = tensor([[                1,       17179869184,                16],
        [        134217728,     4398046511104, -3178808521666707]])
exp(x1) = tensor([[2.7183e+00, 7.3891e+00, 5.4598e+01],
        [2.9810e+03, 5.4598e+01, 1.4841e+02]])
exp(x2) = tensor([[4.0343e+02, 5.8346e+14, 7.3891e+00],
        [8.1031e+03, 1.3188e+09, 7.6948e+23]])
tensor([[[ 0.,  1.,  2.],
         [ 3.,  4.,  5.],
         [ 6.,  7.,  8.]],

        [[ 9., 10., 11.],
         [12., 13., 14.],
         [15., 16., 17.]]], dtype=torch.float64)
torch.Size([2, 3, 3])
tensor([[[ 1,  2,  3],
         [ 4,  5,  6],
         [ 7,  8,  9]],

        [[10, 11, 12],
         [13, 14, 15],
         [16, 17, 18]]])
torch.Size([2, 3, 3])
tensor operation cat res:
tensor([[[ 0.,  1.,  2.],
         [ 3.,  4.,  5.],
         [ 6.,  7.,  8.]],

        [[ 9., 10., 11.],
         [12., 13., 14.],
         [15., 16., 17.]],

        [[ 1.,  2.,  3.],
         [ 4.,  5.,  6.],
         [ 7.,  8.,  9.]],

        [[10., 11., 12.],
         [13., 14., 15.],
         [16., 17., 18.]]], dtype=torch.float64)
torch.Size([4, 3, 3])
tensor([[[ 0.,  1.,  2.],
         [ 3.,  4.,  5.],
         [ 6.,  7.,  8.],
         [ 1.,  2.,  3.],
         [ 4.,  5.,  6.],
         [ 7.,  8.,  9.]],

        [[ 9., 10., 11.],
         [12., 13., 14.],
         [15., 16., 17.],
         [10., 11., 12.],
         [13., 14., 15.],
         [16., 17., 18.]]], dtype=torch.float64)
torch.Size([2, 6, 3])
tensor([[[ 0.,  1.,  2.,  1.,  2.,  3.],
         [ 3.,  4.,  5.,  4.,  5.,  6.],
         [ 6.,  7.,  8.,  7.,  8.,  9.]],

        [[ 9., 10., 11., 10., 11., 12.],
         [12., 13., 14., 13., 14., 15.],
         [15., 16., 17., 16., 17., 18.]]], dtype=torch.float64)
torch.Size([2, 3, 6])
x1 == x2 : tensor([[[False, False, False],
         [False, False, False],
         [False, False, False]],

        [[False, False, False],
         [False, False, False],
         [False, False, False]]])
x1.sum = 153.0
x2.sum = 171
a=tensor([[0],
        [1],
        [2]])
b=tensor([[0, 1]])
a+b=tensor([[0, 1],
        [1, 2],
        [2, 3]])
tensor([[0, 1, 2],
        [3, 4, 5],
        [6, 7, 8]])
x[1]=tensor([3, 4, 5])
x[0:2]=tensor([[0, 1, 2],
        [3, 4, 5]])
x[-1]=tensor([6, 7, 8])
x[1,2] = 99, res = tensor([[ 0,  1,  2],
        [ 3,  4, 99],
        [ 6,  7,  8]])
x[0:2,2] = 199, res = tensor([[  0,   1, 199],
        [  3,   4, 199],
        [  6,   7,   8]])
x[2,0:2] = 299, res = tensor([[  0,   1, 199],
        [  3,   4, 199],
        [299, 299,   8]])
x[0,:] = 888, res = tensor([[888, 888, 888],
        [  3,   4, 199],
        [299, 299,   8]])
140278825928176
140278825746288
140278825746288
```
