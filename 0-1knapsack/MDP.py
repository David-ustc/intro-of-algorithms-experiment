import numpy as np
class box():
    def __init__(self,id,up,down,left,right):
        self.id = id
        self.up = up
        self.down = down
        self.left = left
        self.right = right
class env():
    def __init__(self):
        self.boxes = []
        self.matrix = np.zeros((11,11))*0.0
        self.policy = [1]*11
        self.Qfunction = np.zeros((11,4))*0.0
        self.reward = np.array([0,0,0,0,0,0,0,0,0,-1000,10])*1.0
        self.gamma = 0.9
        self.Vfunction = np.array([0,0,0,0,0,0,0,0,0,0,0])*1.0
        self.boxes.append(box(1,2,1,1,4))
        self.boxes.append(box(2,3,1,2,2))
        self.boxes.append(box(3,3,2,3,5))
        self.boxes.append(box(4,4,4,1,6))
        self.boxes.append(box(5,5,5,3,8))
        self.boxes.append(box(6,7,6,4,9))
        self.boxes.append(box(7,8,6,7,10))
        self.boxes.append(box(8,8,7,5,11))
        self.boxes.append(box(9,10,9,6,9))
        self.boxes.append(box(10,10,10,10,10))
        self.boxes.append(box(11,11,11,11,11))
    def update_matrix(self):
        for i in range(11):
            if self.policy[i] == 1: #left
                self.matrix[self.boxes[i].left-1][i] += 0.8
                self.matrix[self.boxes[i].down-1][i] += 0.05
                self.matrix[self.boxes[i].up-1][i] += 0.05
                self.matrix[self.boxes[i].id-1][i] += 0.1
            elif self.policy[i] == 2: #right
                self.matrix[self.boxes[i].right-1][i] += 0.8
                self.matrix[self.boxes[i].down-1][i] += 0.05
                self.matrix[self.boxes[i].up-1][i] += 0.05
                self.matrix[self.boxes[i].id-1][i] += 0.1
            elif self.policy[i] == 3: #up
                self.matrix[self.boxes[i].up-1][i] += 0.8
                self.matrix[self.boxes[i].left-1][i] += 0.05
                self.matrix[self.boxes[i].right-1][i] += 0.05
                self.matrix[self.boxes[i].id-1][i] += 0.1
            elif self.policy[i] == 4: #down
                self.matrix[self.boxes[i].down-1][i] += 0.8
                self.matrix[self.boxes[i].left-1][i] += 0.05
                self.matrix[self.boxes[i].right-1][i] += 0.05
                self.matrix[self.boxes[i].id-1][i] += 0.1
    def clear_matrix(self):
        self.matrix = np.zeros((11,11))*0.0
    def value_iteration(self):
        for i in range(11):
            for j in range(4):
                self.policy[i] = j + 1
                self.update_matrix()
                self.matrix = np.transpose(self.matrix)
                self.Qfunction[i][j] = self.create_reward(i+1,j+1) + self.gamma * np.sum(self.matrix[i]*self.Vfunction)
                self.clear_matrix()
            self.Vfunction[i] = np.max(self.Qfunction[i])
            self.policy[i] = np.argmax(self.Qfunction[i]) + 1
        return self.Vfunction,self.policy
    def init_state(self):
        self.matrix = np.zeros((11,11))*0.0
        self.policy = [1]*11
        self.Qfunction = np.zeros((11,4))*0.0
    def policy_iteration(self):
        r_e = np.zeros(11)*0.0
        self.clear_matrix()
        self.update_matrix()
        for i in range(11):
            r_e[i] = self.create_reward(i+1,self.policy[i])
        self.matrix = np.transpose(self.matrix)
        self.Vfunction = np.dot(np.linalg.inv(np.eye(11)-self.gamma*self.matrix),np.transpose(r_e))
        self.clear_matrix()
        for i in range(11):
            for j in range(4):
                self.policy[i] = j + 1
                self.update_matrix()
                self.matrix = np.transpose(self.matrix)
                self.Qfunction[i][j] = self.create_reward(i+1,j+1) + self.gamma * np.sum(self.matrix[i]*self.Vfunction)
                self.clear_matrix()
        for i in range(11):
            self.policy[i] = np.argmax(self.Qfunction[i])+1
        return self.Vfunction,self.policy
    def create_reward(self,state,action):
        if action == 1:
            r = 0.8*self.reward[self.boxes[state-1].left-1] + 0.05*self.reward[self.boxes[state-1].down-1]+\
                0.05*self.reward[self.boxes[state-1].up-1] + 0.1*self.reward[self.boxes[state-1].id-1]  
        elif action == 2:
            r = 0.8*self.reward[self.boxes[state-1].right-1] + 0.05*self.reward[self.boxes[state-1].down-1]+\
                0.05*self.reward[self.boxes[state-1].up-1] + 0.1*self.reward[self.boxes[state-1].id-1] 
        elif action == 3:
            r = 0.8*self.reward[self.boxes[state-1].up-1] + 0.05*self.reward[self.boxes[state-1].left-1]+\
                0.05*self.reward[self.boxes[state-1].right-1] + 0.1*self.reward[self.boxes[state-1].id-1]
        elif action == 4:
            r = 0.8*self.reward[self.boxes[state-1].down-1] + 0.05*self.reward[self.boxes[state-1].left-1]+\
                0.05*self.reward[self.boxes[state-1].right-1] + 0.1*self.reward[self.boxes[state-1].id-1]
        return r
MyEnv = env()
MyEnv.init_state()
init_value = np.ones(11)*0.0
init_policy = [1]*11
while True:
    v,p = MyEnv.policy_iteration()
    if (p == init_policy) and (v == init_value).all():
        print("--------------- policy--------------- ")
        print(p)
        print("--------------- value--------------- ")
        print(v)
        break
    init_policy = p
    init_value = v
MyEnv.update_matrix()
MyEnv.init_state()
while True:
    v,p = MyEnv.value_iteration()
    if (v == init_value).all() and (p == init_policy):
        print("--------------- policy--------------- ")
        print(p)
        print("--------------- value--------------- ")
        print(v)
        break
    init_value = v
    init_policy = p

