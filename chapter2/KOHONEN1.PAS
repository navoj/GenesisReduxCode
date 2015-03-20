{$N+}
{$R+}
(* simple kohonen neural network *)

program kohonen1;

(* definitions *)

const
     iunits = 9; (* it is a  good idea to make iunits = ounits *)
     ounits = 9; (* number of neurons *)
     nvectors = 5; (* number of vectors in the training set *)
     alpha = 0.7;

type
    weight_matrix = array[1..iunits,1..ounits] of real; (* weight matrix *)
    invector_matrix = array[1..iunits] of real; (* input vector *)
    ninvector_matrix = array[1..iunits] of real; (* normalized input vector *)
    dvector_matrix = array[1..iunits] of real; (* difference vector *)
    ovector_matrix = array[1..ounits] of real; (* output vector *)

(* definitions *)
var
   i,j,k,v: integer;
   weight: weight_matrix;
   invector: invector_matrix;
   ninvector: ninvector_matrix;
   dvector: dvector_matrix;
   ovector: ovector_matrix;
   counter: integer;
   temp, sigmadif: real;

(* statements *)
begin (* main *)
      (* input a test vector - for test purposes only *)
      for i := 1 to iunits do
      begin (* i loop *)
            write(output, 'input vector element ',i:4,'    ');
            read(input,invector[i]);
      end; (* i loop *)
      writeln;
      writeln;

      (* fill matrix with random numbers *)
      for i := 1 to iunits do
      begin (* i loop *)
            for j := 1 to ounits do
            begin (* j loop *)
                  weight[i,j] := random;
                  writeln(output, weight[i,j]:12:4); (* for test purposes only *)
            end; (* j loop *)
      end; (* i loop *)

      for v := 1 to nvectors do
      begin (* v loop to apply vectors and learn *)
            (* normalize each input vector *)
            for k := 1 to nvectors do
            begin (* k loop for number of vectors *)
                  for i := 1 to iunits do
                  begin (* i loop *)
                        temp := 0;
                        for j := 1 to iunits do
                        begin (* j loop *)
                              temp := temp + sqr(invector[j]);
                        end; (* j loop *)
                        ninvector[i] := invector[i]/sqrt(temp);
                  end; (* i loop *)
            end; (* k loop *)

            (* begin training the kohonen network *)
            (* apply the normalized input vector *)

            { ninvector[i] }
            (* calculate the distance in weight space *)
            for i := 1 to ounits do (* for each neuron *)
            begin (* i loop *)
                  sigmadif := 0;
                  for j := 1 to iunits do (* for each connection *)
                  begin (* j loop *)
                        sigmadif := sigmadif + sqr((ninvector[j] - weight[i][j]));
                  end; (* j loop *)
                  dvector[i] := sigmadif;
            end; (* i loop *)

            (* select winner -- find the largest element in the dvector *)
            temp := 0;
            counter := 0;
            for i := 1 to ounits do
            begin (* i loop *)
                  if (temp < dvector[i]) then
                  begin (* if test *)
                        temp := dvector[i];
                        counter := counter + 1; (* represents the number of the winner *)
                  end; (* if test *)
            end; (* i loop *)

                  (* adjust the weights *)
            for i := 1 to ounits do (* for each neuron *)
            begin (* i loop *)
                        for j := 1 to iunits do (* for each connection *)
                        begin (* j loop *)
                              weight[i,j] := weight[i,j] + alpha*(ninvector[j] - weight[i,j]);
                        end; (* j loop *)
                  end; (* i loop *)
      end; (* end v loop -- go back to apply a new/same vector for learning *)

                  (* print out the weight matrix *)
                  for i := 1 to ounits do (* for each neuron *)
                  begin (* i loop *)
                        for j := 1 to iunits do (* for each connection *)
                            begin (* j loop *)
                                  write(output,weight[i,j]:6:3,'    ');
                            end; (* j loop *)
                        writeln;
                  end; (* i loop *)

                  (* test the network -- how good is it? *)
                  for i := 1 to ounits do (* for each neuron *)
                  begin (* i loop *)
                        ovector[i] := 0;
                        for j := 1 to iunits do (* for each connection *)
                        begin (* j loop *)
                              ovector[i] := ovector[i] + invector[j]*weight[j,i];
                        end; (* j loop *)
                        writeln(output, ovector[i]:12:4);
                  end; (* i loop *)

end. (* main *)


