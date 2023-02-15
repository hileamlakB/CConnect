import npyscreen
import threading
import time


class MySplitApp(npyscreen.NPSApp):
    def main(self):
        # Create the form
        F = npyscreen.Form()
        split = F.add(npyscreen.SplitForm, relx=2, rely=2)
        left = split.add(npyscreen.BoxTitle, name="Left Side")
        right = split.add(npyscreen.BoxTitle, name="Right Side")

        # Create threads to write on both sides
        left_thread = threading.Thread(target=self.write_left, args=(left,))
        right_thread = threading.Thread(target=self.write_right, args=(right,))

        # Start the threads
        left_thread.start()
        right_thread.start()

        # Run the form
        F.edit()

    def write_left(self, widget):
        while True:
            widget.edit()
            widget.footer.value = f"Left side - {time.time()}"
            widget.display()
            time.sleep(1)

    def write_right(self, widget):
        while True:
            widget.edit()
            widget.footer.value = f"Right side - {time.time()}"
            widget.display()
            time.sleep(1)


if __name__ == '__main__':
    App = MySplitApp()
    App.run()
