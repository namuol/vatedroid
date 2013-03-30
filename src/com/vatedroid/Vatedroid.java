package com.vatedroid;

import android.os.AsyncTask;
import android.os.Bundle;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ProgressDialog;
import android.text.TextUtils;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.TextView;

public class Vatedroid extends Activity implements OnClickListener {
  private EditText input;
  private RadioGroup type;
  private TextView output;
  
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.main);
    this.input = (EditText) super.findViewById(R.id.input);
    this.type = (RadioGroup) super.findViewById(R.id.type);
    this.output = (TextView) super.findViewById(R.id.output);
    Button button = (Button) super.findViewById(R.id.button);
    button.setOnClickListener(this);
    
    FibLib.init();
  }
  
  @Override
  protected void onDestroy() {
    FibLib.deinit();
  }
  
  @SuppressLint("DefaultLocale")
  @Override
  public void onClick(View view) {
    String s = this.input.getText().toString();
    
    if (TextUtils.isEmpty(s)) {
      return;
    }
    
    final ProgressDialog dialog = ProgressDialog.show(this, "", "Calculating...", true);
    final long n = Long.parseLong(s);
    
    new AsyncTask<Void, Void, String>() {

      @Override
      protected String doInBackground(Void... params) {
        long result = 0;
        long t = System.currentTimeMillis();
        
        switch (Vatedroid.this.type.getCheckedRadioButtonId()) {
        case R.id.type_fib_jr:
          result = FibLib.fibJR(n);
          break;
        case R.id.type_fib_ji:
          result = FibLib.fibJI(n);
          break;
        case R.id.type_fib_nr:
          result = FibLib.fibNR(n);
          break;
        case R.id.type_fib_ni:
          result = FibLib.fibNI(n);
          break;
        case R.id.type_fib_jsr:
          result = FibLib.fibJSR(n);
          break;
        case R.id.type_fib_jsi:
          result = FibLib.fibJSI(n);
          break;
        }
        
        t = System.currentTimeMillis() - t;
        
        return String.format("fib(%d)=%d in %d ms", n, result, t);
      }

      @Override
      protected void onPostExecute(String result) {
        dialog.dismiss();
        Vatedroid.this.output.setText(result);
      }
      
    }.execute();
  }
}
