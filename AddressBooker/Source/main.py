from gui import *

def main():
    root = Tk()
    root.minsize(width=1100, height=500)
    app = BookWindow(root, 'large_example')
    root.mainloop()

if __name__ == '__main__':
    main()