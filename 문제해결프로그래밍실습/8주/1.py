import numpy as np

class GoldenRectangle:
    def __init__(self, n):
        self.fibonacci_list = self.fibo(n)

    @staticmethod
    def fibo(n):
        # TODO: Implement Fibonacci sequence generation for problem 8-1
        if n <= 0:
            return []
        if n == 1:
            return [1]

        dp = [1, 1]
        for i in range(2, n):
            nex = dp[-1] + dp[-2]
            dp.append(nex)

        return dp
    
    @staticmethod
    def right_operation(grid_cells, cur_rect_length, height, width):
        # TODO: Implement the right operation to fill the grid
        grid_cells[height : height + cur_rect_length, width : width + cur_rect_length] = cur_rect_length
        return grid_cells

    @staticmethod
    def down_operation(grid_cells, cur_rect_length, height, width):
        # TODO: Implement the down operation to fill the grid
        grid_cells[height : height + cur_rect_length, width : width + cur_rect_length] = cur_rect_length
        return grid_cells
    
    @staticmethod
    def left_operation(grid_cells, cur_rect_length, height, width):
        # TODO: Implement the left operation to fill the grid
        grid_cells[height : height + cur_rect_length, width : width + cur_rect_length] = cur_rect_length
        return grid_cells

    @staticmethod
    def up_operation(grid_cells, cur_rect_length, height, width):
        # TODO: Implement the up operation to fill the grid
        grid_cells[height : height + cur_rect_length, width : width + cur_rect_length] = cur_rect_length
        return grid_cells

    # Do not change this function name (get_golden_rectangle)
    def get_golden_rectangle(self):
        # TODO: Implement the logic to create the golden rectangle grid
        height, width = self.fibonacci_list[-1], self.fibonacci_list[-1] + self.fibonacci_list[-2]  
        grid_cells = np.zeros((height, width))
        
        fib_list = self.fibonacci_list
        n = len(fib_list)
        if n == 0:
            return np.zeros((0, 0))
        
        if n == 1:
            return np.array([[1]])
        
        h, w = 0, 0
        op = [self.right_operation, self.down_operation, self.left_operation, self.up_operation]

        for i in range(n - 1, -1, -1):
            cur = fib_list[i]
            step = (n - 1 - i)
            op_idx = step % 4
            o = op[op_idx]
            nex_cur = 0
            if i > 0:
                nex_cur = fib_list[i - 1]
            if op_idx == 0:
                draw_h, draw_w = h, w
                o(grid_cells, cur, draw_h, draw_w)
                w = w + cur
            elif op_idx == 1:
                draw_h, draw_w = h, w
                o(grid_cells, cur, draw_h, draw_w)
                h = h + cur
            elif op_idx == 2:
                draw_h, draw_w = h, w + nex_cur
                o(grid_cells, cur, draw_h, draw_w)
                h = h + (cur - nex_cur)
            elif op_idx == 3:
                draw_h, draw_w = h, w
                o(grid_cells, cur, draw_h, draw_w)
                h = h - nex_cur
        
        
        
    
        # Return the grid cells as an (H, W) ndarray
        return grid_cells
        
# Instructions: Complete only the class implementation. You may add print statements to test when using the "Run" button, 
# but only the class will be used for grading upon submission.

# n = 7
# golden_rectangle_instance = GoldenRectangle(n)
# golden_rectangle = golden_rectangle_instance.get_golden_rectangle()
# print(golden_rectangle)

# Explanation on the difference with @staticmethod:
# Using @staticmethod:
#   - Methods marked with @staticmethod do not need access to any instance-specific data, 
#     so they can be called directly from the class, e.g., GoldenRectangle.fibo(10).
#   - They do not access self or instance variables.
# Without @staticmethod:
#   - Methods without @staticmethod need an instance of the class to access instance-specific data (like self.fibonacci_list).
#   - They are called through an instance, e.g., golden_rectangle_instance.get_golden_rectangle().

