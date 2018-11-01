package tina.com.player;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.bumptech.glide.Glide;
import com.tina.live.list.Items;
import com.tina.live.list.LiveList;

import java.util.ArrayList;
import java.util.List;

/**
 * @author Lance
 * @date 2018/9/7
 */
public class LiveAdapter extends RecyclerView.Adapter<LiveAdapter.MyHolder> implements View
        .OnClickListener {


    private LayoutInflater layoutInflater;
    private List<Items> items;
    private OnItemClickListener mItemClickListener;


    public LiveAdapter(Context context) {
        layoutInflater = LayoutInflater.from(context);
        items = new ArrayList<>();
    }

    @NonNull
    @Override
    public MyHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int i) {
        View view = layoutInflater.inflate(R.layout.item_room, viewGroup, false);
        //点击
        view.setOnClickListener(this);
        return new MyHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull MyHolder myHolder, int i) {
        Items items = this.items.get(i);
        myHolder.title.setText(items.getName());
        Glide.with(myHolder.picture).load(items.getPictures().getImg()).into(myHolder.picture);
        //设置标签
        myHolder.itemView.setTag(items.getId());
    }

    @Override
    public int getItemCount() {
        return items.size();
    }

    /**
     * 设置新数据
     *
     * @param liveList
     */
    public void setLiveList(LiveList liveList) {
        items.clear();
        items.addAll(liveList.getData().getItems());
    }

    /**
     * 点击回调
     *
     * @param mItemClickListener
     */
    public void setItemClickListener(OnItemClickListener mItemClickListener) {
        this.mItemClickListener = mItemClickListener;
    }

    @Override
    public void onClick(View v) {
        if (mItemClickListener != null) {
            mItemClickListener.onItemClick((String) v.getTag());
        }
    }

    public interface OnItemClickListener {
        void onItemClick(String id);
    }


    class MyHolder extends RecyclerView.ViewHolder {

        ImageView picture;
        TextView title;

        public MyHolder(@NonNull View itemView) {
            super(itemView);
            picture = itemView.findViewById(R.id.picture);
            title = itemView.findViewById(R.id.title);
        }
    }
}
