import Api
import detect

class Model:
    def __init__(self):
        self.model = {}

    def append_node(self, node):
        if node.funcName not in self.model:
            self.model[node.funcName] = node
            return
        self.model[node.funcName].merge_dependency(node)

    def print(self):
        for node in self.model:
            print(node)
            i = 0
            for param in self.model[node].params:
                if param.hasParent:
                    print(i)
                    print(param.parents)
                i += 1
            print()

    def make_depDict(self):
        self.depLists = dict()
        for node in self.model:
            params = list()
            depList = {node: params}

            for param in self.model[node].params:
                if param.hasParent:
                    params.append(param.parents)
                #elif param.isStruct:
                #    node.funcName
                #    pass
                #elif param.isHandle:
                #    pass
                #elif param.isPointer:
                #    pass
                elif param.isConst:
                    params.append(param.constValue)
                else:
                    params.append(['pass'])
            self.depLists.update({node: params})
        return self.depLists

class Param:
    def __init__(self):
        self.hasParent = False
        self.isConst = False
        self.constValue = []
        self.parents = []

    def append_parent(self, parent, indexParam):
        self.parents.append([parent, indexParam])

class Node:
    def __init__(self, log):
        self.funcName = log.funcName
        self.params = [Param() for i in range(len(log.params))]
        self.depId = 0

    def __eq__(self, other):
        return self.funcName == other.funcName

    def merge_dependency(self, other):

        for i in range(len(self.params)):
            self.params[i].parents.sort()
            other.params[i].parents.sort()
            if self.params[i].parents != other.params[i].parents:

                for parent in other.params[i].parents:
                    if parent not in self.params[i].parents:
                        self.params[i].parents.append(parent)
                self.params[i].hasParent = True

    def set_dependency(self, i, parent, indexParam):  # set Dependency between self's ith arg with parent
        if not self.params[i].hasParent:  # if ith param doesnt have parent
            self.params[i].hasParent = True

        if [parent, indexParam] not in self.params[i].parents:  # if ith param doesnt already have same parent
            self.params[i].append_parent(parent, indexParam)

    def set_const(self, i, value):
        self.params[i].isConst = True
        self.params[i].constValue = value