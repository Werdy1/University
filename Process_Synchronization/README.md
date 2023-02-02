*****
Чотири дочірніх процеси виконують деякі цикли робіт, передаючи після закінчення чергового циклу через один і 
той же сегмент поділюваної пам'яті батьківському процесові черговий рядок деякого віршу, 
при цьому перший процес передає 1-й, 5-й, 9-й рядки, другий — 2-й, 6-й, 10-й рядки, 
третій — 3-й, 7-й, 11-й рядки, четвертий — 4-й, 8-й, 12-й рядки. 
Цикли робіт процесів не збалансовані за часом. Батьківський процес компонує з переданих фрагментів 
закінчений вірш і виводить його після закінчення роботи всіх дочірніх процесів. 
Задача розв' язана з використанням апарату семафорів.

*****
Four child processes perform several cycles of work, passing after the end of the next cycle through the 
same segment of shared memory to the parent process another line of some poem, 
while the first process transmits the 1st, 5th, 9th lines, the second — 2nd, 6th, 10th rows, 
third - 3rd, 7th, 11th rows, fourth - 4th, 8th, 12th rows. 
Work cycles of processes are not balanced in time. The parent process composes a finished poem 
from the transferred fragments and outputs it after the work of all child processes has finished.
The problem is solved using the semaphore apparatus.

*****