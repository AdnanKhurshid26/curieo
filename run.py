import subprocess
import sys

def main():
    if len(sys.argv) != 3:
        print("Usage: python run.py <input_file> <output_file>")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    cpp_program = './main'

    try:
        subprocess.run([cpp_program, input_file, output_file], check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except subprocess.CalledProcessError as e:
        print(f"stderr: {e.stderr.decode()}")

if __name__ == "__main__":
    main()
