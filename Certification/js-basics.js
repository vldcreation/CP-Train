function weekdayText(weekdays) {
    return num => {
        if(Number(num) > weekdays.length){
            throw new Error('Invalid weekday number')
        }
        return weekdays[Number(num)]
    }
}

weeks = ["Mon","Tue","Wed","Thu", "Fri", "Sat", "Sun"]
const getText = weekdayText(weeks)
const value = getText(1)
console.log(value)