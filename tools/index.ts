
import xlsx from 'node-xlsx';
import fs from "fs/promises";
import Decimal from "decimal.js";

interface SheetRow {
    partID : string;
    name: string;


};

function extractNumberWithSurfix(number_string_with_surfix) : number
{
    var reg_result = /([0-9kMm]+?)Ω/.exec(number_string_with_surfix);

    number_string_with_surfix = reg_result[1];


    var reg_result = /([0-9]+?)k/.exec(number_string_with_surfix);

    if (reg_result)
    {
        return  Decimal(reg_result[1]).mul(1000).toNumber();
    }

    reg_result = /([0-9]+?)M/.exec(number_string_with_surfix);

    if (reg_result)
    {
        return  Decimal(reg_result[1]).mul(1000000).toNumber();
    }
    reg_result = /([0-9]+?)m/.exec(number_string_with_surfix);

    if (reg_result)
    {
        return  Decimal(reg_result[1]).div(1000).toNumber();
    }
    return Decimal(number_string_with_surfix).toNumber();
}

async function main() {

    const workSheetsFromFile = xlsx.parse(`${__dirname}/../免换料费专区.xlsx`);

    const SheetData : any[][] = workSheetsFromFile[0].data;

    const OnlyResistor = SheetData.filter( (value : any[]) => {
        const reg_result = /([0-9kMm]+?)Ω ±/.exec(value[1]);
        return reg_result&& reg_result.length > 1
     })

    console.log(OnlyResistor);


    // now sort it !

     OnlyResistor.sort( (a: any[], b:any[])=>{
        return extractNumberWithSurfix(a[1]) - extractNumberWithSurfix(b[1]);
     });

     console.log(OnlyResistor);


     const Carray = OnlyResistor.map((value : string[])=>{

        return  [
            extractNumberWithSurfix(value[1]), value[1], value[0]
        ];
     });

     console.log(Carray);


     Carray.forEach( (value)=>{
        console.log(`{ ${value[0]}, "${value[1]}", "${value[2]}" },`);
    });

}

main()
